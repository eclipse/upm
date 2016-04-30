/*
 * Author: Zion Orent <sorent@ics.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <iostream>
#include <string>
#include <stdexcept>
#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <sys/sysinfo.h>
#include "ppd42ns.hpp"

using namespace upm;

PPD42NS::PPD42NS(int pin)
{
    if ( !(m_gpio = mraa_gpio_init(pin)) ) 
      {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_gpio_init() failed, invalid pin?");
        return;
      }

    mraa_gpio_dir(m_gpio, MRAA_GPIO_IN);
}

PPD42NS::~PPD42NS()
{
    mraa_gpio_close(m_gpio);
}

// Assues density, shape, and size of dust to estimate mass concentration from particle count.
//
// This method was described in a 2009 paper
// Preliminary Screening System for Ambient Air Quality in Southeast Philadelphia by Uva, M., Falcone, R., McClellan, A., and Ostapowicz, E.
// http://www.cleanair.org/sites/default/files/Drexel%20Air%20Monitoring_-_Final_Report_-_Team_19_0.pdf
//
// This method does not use the correction factors, based on the presence of humidity and rain in the paper.
//
// convert from particles/0.01 ft3 to μg/m3
double pcs2ugm3 (double concentration_pcs)
{
    double pi = 3.14159;
    // All particles are spherical, with a density of 1.65E12 µg/m3
    double density = 1.65 * pow (10, 12);
    // The radius of a particle in the PM2.5 channel is .44 µm
    double r25 = 0.44 * pow (10, -6);
    double vol25 = (4/3) * pi * pow (r25, 3);
    double mass25 = density * vol25; // ug
    double K = 3531.5; // per m^3 

    return concentration_pcs * K * mass25;
}

// https://www3.epa.gov/airquality/particlepollution/2012/decfsstandards.pdf
static struct aqi {
    float clow;
    float chigh;
    int llow;
    int lhigh;
} aqi[] = {
  {0.0,    12.4,   0, 50},
  {12.1,   35.4,  51, 100},
  {35.5,   55.4, 101, 150},
  {55.5,  150.4, 151, 200},
  {150.5, 250.4, 201, 300},
  {250.5, 350.4, 301, 350},
  {350.5, 500.4, 401, 500},
};

// Guidelines for the Reporting of Daily Air Quality – the Air Quality Index (AQI)
// https://www3.epa.gov/ttn/oarpg/t1/memoranda/rg701.pdf
//
// Revised air quality standards for particle pollution and updates to the air quality index (aqi)
// https://www3.epa.gov/airquality/particlepollution/2012/decfsstandards.pdf
//
// calculate AQI (Air Quality Index) based on μg/m3 concentration
int ugm32aqi (double ugm3)
{
  int i;

  for (i = 0; i < 7; i++) {
    if (ugm3 >= aqi[i].clow &&
        ugm3 <= aqi[i].chigh) {
        // Ip =  [(Ihi-Ilow)/(BPhi-BPlow)] (Cp-BPlow)+Ilow,
        return ((aqi[i].lhigh - aqi[i].llow) / (aqi[i].chigh - aqi[i].clow)) * 
            (ugm3 - aqi[i].clow) + aqi[i].llow;
    }
  }

  return 0;
}

dustData PPD42NS::getData()
{
	dustData data;
	struct timespec printData_start={0,0};
	struct timespec printData_now={0,0};
	clock_gettime(CLOCK_MONOTONIC, &printData_start);
	clock_gettime(CLOCK_MONOTONIC, &printData_now);
	double low_pulse_occupancy = 0;
	double pulse_check_time = 30;
	
	// Keep reading dust data until 30 seconds have passed
	double start_time, end_time;
	while (m_timediff(printData_start, printData_now) < pulse_check_time)
	{
		start_time = m_timediff(printData_start, printData_now);
		end_time = pulse_check_time - start_time;
		low_pulse_occupancy += pulseIn_polyfill(0, end_time);
		clock_gettime(CLOCK_MONOTONIC, &printData_now);
	}

	// Store dust data
	double ratio = low_pulse_occupancy / (pulse_check_time * 1000 * 10.0);  // Integer percentage 0=>100
	double concentration = (1.1 * pow(ratio,3)) - (3.8 * pow(ratio, 2)) + (520 * ratio) + 0.62; // using spec sheet curve
	data.lowPulseOccupancy = (int)low_pulse_occupancy;
	data.ratio = ratio;
	data.concentration = concentration;
    data.ugm3 = pcs2ugm3(data.concentration);
    data.aqi = ugm32aqi(data.ugm3);
	return data;
}

// Mimicking Arduino's pulseIn function
//	return how long it takes a pin to go from HIGH to LOW or LOW to HIGH
double PPD42NS::pulseIn_polyfill(bool high_low_value, double end_time)
{
	struct timespec pulseIn_start={0,0};
	struct timespec pulseIn_now={0,0};
	struct timespec pulsetime_start={0,0};
	struct timespec pulsetime_end={0,0};

	int pin_val = 5; // some non-zero, non-1 number
	bool started_timing = false;
	bool ended_timing = false;
	clock_gettime(CLOCK_MONOTONIC, &pulseIn_start);

	// run through this loop until either:
	//	a) we detect a change in pulse
	//	b) we've hit 30 seconds
	while (!ended_timing && (m_timediff(pulseIn_start, pulseIn_now) < end_time))
	{
		pin_val = (bool)mraa_gpio_read(m_gpio);
		if (pin_val == high_low_value && !started_timing)
		{
			clock_gettime(CLOCK_MONOTONIC, &pulsetime_start);
			started_timing = true;
			//std::cout << "Started counting pulse change" << std::endl;
			usleep(50);
		}
		else if (started_timing && pin_val != high_low_value)
		{
			clock_gettime(CLOCK_MONOTONIC, &pulsetime_end);
			ended_timing = true;
			//std::cout << "Ended counting pulse change" << (m_timediff(pulseIn_start, pulseIn_now)) << std::endl;
		}
		else
			usleep(50);
		clock_gettime(CLOCK_MONOTONIC, &pulseIn_now);
	}

	double low_pulse_occupancy = 0;

	// if we ended due to detecting a pulse change and not due to hitting 30 seconds
	if (started_timing && ended_timing)
		low_pulse_occupancy = m_timediff(pulsetime_start, pulsetime_end);
	//std::cout << "Low pulse occupancy is " << low_pulse_occupancy << " seconds" << std::endl;

	return (low_pulse_occupancy * 1000000); // convert to microseconds
}

double PPD42NS::m_timediff(timespec time1, timespec time2)
{
	return ((double)time2.tv_sec + 1.0e-9*time2.tv_nsec) - 
			((double)time1.tv_sec + 1.0e-9*time1.tv_nsec);
}
