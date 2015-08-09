/*
 * Author: Zion Orent <zorent@ics.com>
 * Copyright (c) 2015 Intel Corporation.
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
#include <mraa/aio.h>

namespace upm {
  /**
   * @brief Grove GSR Galvanic Skin Response Sensor library
   * @defgroup grovegsr libupm-grovegsr
   * @ingroup seeed analog electric
   */

  /**
   * @library grovegsr
   * @sensor grovegsr
   * @comname Grove GSR Sensor
   * @type electric
   * @man seeed
   * @con analog
   *
   * @brief API for the Grove GSR Galvanic Skin Response Sensor
   * 
   * Measures the electrical conductance of skin
   * to measure strong emotional reactions.
   * In other words, it measures sweat on your fingers
   * as an indicator of strong emotional reactions.
   *
   * @image html grovegsr.jpg
   * @snippet grovegsr.cxx Interesting
   */
  class GroveGSR {
  public:
    /**
     * Grove GSR sensor constructor
     *
     * @param pin Analog pin to use
     */
    GroveGSR(int pin);
    /**
     * GroveGSR destructor
     */
    ~GroveGSR();

    /**
     * Calibrates the Grove GSR sensor
     */
    void calibrate();

    /**
     * Gets the electrical conductance of the skin from the sensor
     *
     * @return Electrical conductance of the skin
     */
    int value();

  private:
    mraa_aio_context m_aio;
  };
}


