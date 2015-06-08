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
   * @brief GroveEMG Muscle Signal reader sensor library
   * @defgroup groveemg libupm-groveemg
   * @ingroup seeed analog electric
   */

  /**
   * @library groveemg
   * @sensor groveemg
   * @comname Grove EMG Sensor
   * @type electric
   * @man seeed
   * @con analog
   *
   * @brief API for the GroveEMG Muscle Signal Reader Sensor
   * 
   * GroveEMG Muscle Signal reader gathers small muscle signals,
   * then processes and returns the result
   *
   * @image html groveemg.jpg 
   * @snippet groveemg.cxx Interesting
   */
  class GroveEMG {
  public:
    /**
     * GroveEMG sensor constructor
     *
     * @param pin analog pin to use
     */
    GroveEMG(int pin);
    /**
     * GroveEMG Destructor
     */
    ~GroveEMG();

    /**
     * Calibrate the GroveEMG Sensor
     */
    void calibrate();

    /**
     * Measure the muscle signals from the sensor
     *
     * @return the muscle output as analog voltage
     */
    int value();

  private:
    mraa_aio_context m_aio;
  };
}


