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
#define GROVEELECTRICITY_ADC_RES 1024

namespace upm {

  /**
   * @brief C++ API for the Grove Electricity Sensor
   *
   * UPM module for the Grove Electricity Sensor 
   * This module can measure AC current moving through a wire at up 
   * to 5A.
   *
   * @ingroup grove analog
   * @snippet groveelectricity.cxx Interesting
   */
  class GroveElectricity {
  public:
    /**
     * Grove Electricity sensor constructor
     *
     * @param pin analog pin to use
     */
    GroveElectricity(int pin);

    /**
     * Grove Electricity Destructor
     */
    ~GroveElectricity();

    /**
     * Return the number of milliseconds elapsed since initClock()
     * was last called.
     *
     * @return elapsed milliseconds
     */
    uint32_t getMillis();

    /**
     * Reset the Clock
     *
     */
    void initClock();

    /**
     * Get the conversion value from the sensor
     *
     * @return the highest value obtained over 1 second of measuring
     */
    unsigned int highestValue();

    /**
     * Compute the measured voltage
     *
     * @param val value measured by highestValue()
     * @param res ADC resolution
     *
     * @return the The measured current in milliamps
     */
    float milliAmps(unsigned int val, int res=GROVEELECTRICITY_ADC_RES);

  private:
    struct timeval m_startTime;
    mraa_aio_context m_aio;
  };
}


