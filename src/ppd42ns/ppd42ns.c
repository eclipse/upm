/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Contributions: Rex Tsai <rex.cc.tsai@gmail.com>
 *                Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * Rewritten Based on original C++ driver written by:
 * Author: Zion Orent <sorent@ics.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#include <upm_math.h>
#include <upm_utilities.h>

#include "ppd42ns.h"

// Returns the amount of time it takes a pin to go from HIGH to LOW or
// from LOW to HIGH
static uint32_t ppd42ns_pulse_in(const ppd42ns_context dev,
                                 bool high_low_value);
double pcs2ugm3 (double concentration_pcs);

ppd42ns_context ppd42ns_init(int pin)
{
    ppd42ns_context dev =
        (ppd42ns_context)malloc(sizeof(struct _ppd42ns_context));

    if (!dev)
        return NULL;

    dev->gpio = NULL;

    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        ppd42ns_close(dev);
        return NULL;
    }

    // MRAA contexts...
    if ( !(dev->gpio = mraa_gpio_init(pin)) )
    {
        printf("%s: mraa_gpio_init() failed\n",
               __FUNCTION__);
        ppd42ns_close(dev);
        return NULL;
    }

    mraa_gpio_dir(dev->gpio, MRAA_GPIO_IN);

    return dev;
}

void ppd42ns_close(ppd42ns_context dev)
{
    assert(dev != NULL);

    if (dev->gpio)
        mraa_gpio_close(dev->gpio);

    free(dev);
}

ppd42ns_dust_data ppd42ns_get_data(const ppd42ns_context dev)
{
    assert(dev != NULL);

    ppd42ns_dust_data data;

    // in ms, 30 seconds
    const unsigned int pulse_check_time = 30000;
    // loop timer
    upm_clock_t max_loop_time;

    unsigned int low_pulse_occupancy = 0;

    max_loop_time = upm_clock_init();

    do {
        low_pulse_occupancy += ppd42ns_pulse_in(dev, 0);
    } while (upm_elapsed_ms(&max_loop_time) < pulse_check_time);

    // Store dust data
    // Integer percentage 0=>100
    double ratio = (float)low_pulse_occupancy
        / ((float)pulse_check_time * 10.0);

     // using spec sheet curve
    double concentration = (1.1 * pow(ratio,3)) - (3.8 * pow(ratio, 2))
        + (520 * ratio) + 0.62;

    data.lowPulseOccupancy = low_pulse_occupancy;
    data.ratio = ratio;
    data.concentration = concentration;
    data.ugm3 = pcs2ugm3(data.concentration);
    data.aqi = upm_ugm3_to_aqi(data.ugm3);

    return data;
}


// Mimicking Arduino's pulseIn function
// return how long it takes a pin to go from HIGH to LOW or LOW to HIGH
static uint32_t ppd42ns_pulse_in(const ppd42ns_context dev,
                                 bool high_low_value)
{
    assert(dev != NULL);

    // we run for no more than 1 second at a time
    upm_clock_t pulse_time;
    uint32_t total_pulse_time = 0;

    upm_clock_t max_time = upm_clock_init();
    bool pin_level;
    bool is_timing = false;

    do {
        pin_level = (bool)mraa_gpio_read(dev->gpio);

        if (!is_timing && pin_level == high_low_value)
        {
            // level is desired level, but not currently timing
            pulse_time = upm_clock_init();
            is_timing = true;
        }
        else if (is_timing && pin_level != high_low_value)
        {
            // we started timing, but level changed
            total_pulse_time += upm_elapsed_us(&pulse_time);
            is_timing = false;
        }
        else
        {
            // not timing and/or level is not equal to desired level
            // so we "wait".
            upm_delay_us(10);
        }
    } while (upm_elapsed_ms(&max_time) < 1000); // 1 second

    if (is_timing)
    {
        // if we were still timing when the loop expired, add the
        // accumulated time.
        total_pulse_time += upm_elapsed_us(&pulse_time);
    }

    return total_pulse_time;
}

// Assumes density, shape, and size of dust to estimate mass concentration from particle count.
//
// This method was described in a 2009 paper
// Preliminary Screening System for Ambient Air Quality in Southeast Philadelphia by Uva, M., Falcone, R., McClellan, A., and Ostapowicz, E.
// https://www.yumpu.com/en/document/view/31692906/preliminary-screening-system-for-ambient-air-quality-in-southeast-
//
// This method does not use the correction factors, based on the presence of humidity and rain in the paper.
//
// convert from particles/0.01 ft3 to ug/m3
double pcs2ugm3 (double concentration_pcs)
{
    double pi = 3.14159;
    // All particles are spherical, with a density of 1.65E12 ug/m3
    double density = 1.65 * pow (10, 12);
    // The radius of a particle in the PM2.5 channel is .44 um
    double r25 = 0.44 * pow (10, -6);
    double vol25 = (4/3) * pi * pow (r25, 3);
    double mass25 = density * vol25; // ug
    double K = 3531.5; // per m^3 

    return concentration_pcs * K * mass25;
}
