/*
 * Author: Jon Trulson <jtrulson@ics.com>
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
#include <iostream>
#include <stdint.h>
#include <sys/time.h>
#include <mraa/aio.h>

// default ADC resolution. 
#define TA12200_ADC_RES 1024

namespace upm {
/**
 * @brief TA12-200 Current Transformer library
 * @defgroup ta12200 libupm-ta12200
 * @ingroup seeed analog electric
 */
/**
 * @library ta12200
 * @sensor ta12200
 * @comname TA12-200 Current Transformer
 * @altname Grove Electricity Sensor
 * @type electric
 * @man seeed
 * @web http://www.seeedstudio.com/wiki/Grove_-_Electricity_Sensor
 * @con analog
 *
 * @brief API for the TA12-200 Current Transformer
 * 
 *   UPM module for the TA12-200 current transformer found,
 *   for instance, in the Grove Electricity Sensor. 
 *   This module can measure AC moving through a wire at up 
 *   to 5 A.
 *
 * @image html ta12200.jpg
 * @snippet ta12200.cxx Interesting
 */
  class TA12200 {
  public:
    /**
     * TA12200 constructor
     *
     * @param pin Analog pin to use
     */
    TA12200(int pin);

    /**
     * TA12200 destructor
     */
    ~TA12200();

    /**
     * Returns the number of milliseconds elapsed since initClock()
     * was last called.
     *
     * @return Elapsed milliseconds
     */
    uint32_t getMillis();

    /**
     * Resets the clock
     *
     */
    void initClock();

    /**
     * Gets the conversion value from the sensor
     *
     * @return Highest value obtained over 1 second of measuring
     */
    unsigned int highestValue();

    /**
     * Computes the measured voltage
     *
     * @param val Value measured by highestValue()
     * @param res ADC resolution
     *
     * @return Measured current in mA
     */
    float milliAmps(unsigned int val, int res=TA12200_ADC_RES);

  private:
    struct timeval m_startTime;
    mraa_aio_context m_aio;
  };
}


