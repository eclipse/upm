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
   * @defgroup o2 libupm-o2
   * @ingroup seeed analog gaseous
   */

  /**
   * @library o2
   * @sensor o2
   * @comname Oxygen (O2) Concentration Sensor
   * @altname Grove O2 Sensor
   * @type gaseous
   * @man seeed
   * @con analog
   * @web http://wiki.seeed.cc/Grove-Gas_Sensor-O2/
   *
   * @brief API for the Grove O2 Oxygen Gas Sensor
   *
   * The Grove O2 Oxygen Gas sensor measures the oxygen concentration in the air
   *
   * @image html o2.jpg
   * @snippet o2.cxx Interesting
   */
  class O2 {
  public:
    /**
     * Grove O2 Oxygen Gas sensor constructor
     *
     * @param pin Analog pin to use
     */
    O2(int pin);
    /**
     * O2 destructor
     */
    ~O2();
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
