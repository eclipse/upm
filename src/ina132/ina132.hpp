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
   * @brief INA132 Differential Amplifier Sensor
   * @defgroup ina132 libupm-ina132
   * @ingroup seeed analog electric
   */
  /**
   * @library ina132
   * @sensor ina132
   * @comname Differential Amplifier
   * @altname INA132
   * @type electric
   * @man seeed
   * @con analog
   * @web http://wiki.seeed.cc/Grove-Differential_Amplifier_v1.2/
   *
   * @brief API for the INA132 Differential Amplifier Sensor
   *
   * The INA132 Differential Amplifier Sensor is designed
   * for precise differential-input amplification.
   *
   * This sensor was tested amplifying the signal
   * from a Weight Sensor (Load Cell) 0-500g
   *
   * @image html ina132.jpg
   * @snippet ina132.cxx Interesting
   */
  class INA132 {
  public:
    /**
     * INA132 constructor
     *
     * @param pin Analog pin to use
     */
    INA132(int pin);
    /**
     * INA132 destructor
     */
    ~INA132();

    /**
     * Measures precise differential-input amplification from the sensor
     *
     * @return Precise differential-input amplification
     */
    float value();

  private:
    mraa_aio_context m_aio;
  };
}


