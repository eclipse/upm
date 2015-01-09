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
#include <mraa/aio.h>

// ref voltage in millivolts
#define GROVEVDIV_VREF  4980

// default ADC resolution. 
#define GROVEVDIV_ADC_RES 1024

namespace upm {

  /**
   * @brief C++ API for the Grove Voltage Divider Sensor
   *
   * UPM module for the Grove Voltage Divider Sensor
   *
   * @ingroup grove analog
   * @snippet grovevdiv.cxx Interesting
   */
  class GroveVDiv {
  public:
    /**
     * Grove Voltage Divider sensor constructor
     *
     * @param pin analog pin to use
     */
    GroveVDiv(int pin);

    /**
     * Grove Voltage Divider Destructor
     */
    ~GroveVDiv();

    /**
     * Get the conversion value from the sensor
     *
     * @param samples specifies how many samples to average over
     * @return the averaged ADC conversion value
     */
    unsigned int value(unsigned int samples);

    /**
     * Compute the measured voltage
     *
     * @param gain gain switch, either 3 or 10 for grove
     * @param val measured voltage (from value())
     * @param vref reference voltage in millivolts
     * @param res ADC resolution
     *
     * @return the measured voltage
     */
    float computedValue(uint8_t gain, uint16_t val, int vref=GROVEVDIV_VREF, 
                        int res=GROVEVDIV_ADC_RES);

  private:
    mraa_aio_context m_aio;
  };
}


