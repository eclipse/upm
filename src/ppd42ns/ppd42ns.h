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
#pragma once

#include <string>
#include <time.h>
#include <mraa/aio.h>

namespace upm {

typedef struct
{
	int lowPulseOccupancy;
	double ratio;
	double concentration;
} dustData;

  /**
   * @brief PPD42NS dust sensor library
   * @defgroup ppd42ns libupm-ppd42ns
   * @ingroup seeed gpio other eak
   */
  /**
   * @library ppd42ns
   * @sensor ppd42ns
   * @comname PPD42NS Dust Sensor
   * @altname Grove Dust Sensor
   * @type other
   * @man seeed
   * @web http://www.seeedstudio.com/wiki/Grove_-_Dust_Sensor
   * @con gpio
   * @kit eak
   *
   * @brief API for the Dust Sensor
   *
   * UPM module for the Dust Sensor
   *
   * @image html ppd42ns.jpg
   * @snippet ppd42ns.cxx Interesting
   */
  class PPD42NS {
  public:
    /**
     * Dust Sensor Constructor
     *
     * @param pin digital pin to use
     */
    PPD42NS(int pin);
    /**
     * Dust Sensor Destructor
     */
    ~PPD42NS();
    /**
     * Print dust concentration
     *
     * @return struct dustData  contains data from dust sensor
     */
     dustData getData();

  private:
        mraa_gpio_context m_gpio;
		/**
		 * Return the amount of time it takes a pin to go from HIGH to LOW or LOW to HIGH
		 *
		 * @param highLowValue int do we measure from HIGH to LOW or LOW to HIGH. highLowValue is "from" value
		 */
		double pulseIn_polyfill(bool highLowValue, double endTime);
		double m_timediff(timespec time1, timespec time2);
	};
}


