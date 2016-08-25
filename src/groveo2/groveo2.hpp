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
   * @brief Grove O2 Oxygen Gas Sensor library
   * @defgroup groveo2 libupm-groveo2
   * @ingroup seeed analog gaseous
   */

  /**
   * @library groveo2
   * @sensor groveo2
   * @comname Grove O2 Sensor
   * @type gaseous
   * @man seeed
   * @con analog
   *
   * @brief API for the Grove O2 Oxygen Gas Sensor
   *
   * The Grove O2 Oxygen Gas sensor measures the oxygen concentration in the air
   *
   * @image html groveo2.jpg    
   * @snippet groveo2.cxx Interesting
   */
  class GroveO2 {
  public:
    /**
     * Grove O2 Oxygen Gas sensor constructor
     *
     * @param pin Analog pin to use
     */
    GroveO2(int pin);
    /**
     * GroveO2 destructor
     */
    ~GroveO2();
    /**
     * Measures O2 from the sensor
     *
     * @return Oxygen concentration as voltage
     */
    float voltageValue();

  private:
    mraa_aio_context m_aio;
  };
}
