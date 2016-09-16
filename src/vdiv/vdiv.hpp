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

// reference voltage in millivolts
#define VDIV_VREF  4980

// default ADC resolution
#define VDIV_ADC_RES 1024

namespace upm {
  /**
   * @brief Voltage Divider Sensor library
   * @defgroup vdiv libupm-vdiv
   * @ingroup seeed analog electric robok
   */

  /**
   * @library vdiv
   * @sensor vdiv
   * @comname Voltage Divider
   * @altname Grove Voltage Divider
   * @type electric
   * @man seeed
   * @con analog
   * @kit robok
   *
   * @brief API for the Voltage Divider Sensor
   *
   * UPM module for the Voltage Divider sensor
   *
   * @image html vdiv.jpg
   * @snippet vdiv.cxx Interesting
   */
  class VDiv {
  public:
    /**
     * Voltage Divider sensor constructor
     *
     * @param pin Analog pin to use
     */
    VDiv(int pin);

    /**
     * Voltage Divider destructor
     */
    ~VDiv();

    /**
     * Gets the conversion value from the sensor
     *
     * @param samples Specifies how many samples to average over
     * @return Average ADC conversion value
     */
    unsigned int value(unsigned int samples);

    /**
     * Computes the measured voltage
     *
     * @param gain Gain switch, example: either 3 or 10 grove vdiv
     * @param val Measured voltage (from value())
     * @param vref Reference voltage in millivolts
     * @param res ADC resolution
     *
     * @return Measured voltage
     */
    float computedValue(uint8_t gain, unsigned int val, int vref=VDIV_VREF,
                        int res=VDIV_ADC_RES);

  private:
    mraa_aio_context m_aio;
  };
}
