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
     * @brief UPM module for the GroveO2 Oxygen Gas Sensor
     * @defgroup groveO2 libupm-groveO2
    */
    /**
     * @brief C++ API for the GroveO2 Oxygen Gas Sensor
     *
     * The Grove O2 sensor measures the oxygen concentration in the air
     * 
     * @ingroup analog groveO2
     * @snippet groveO2.cxx Interesting
    */
  class GroveO2 {
  public:
    /**
     * GroveO2 sensor constructor
     *
     * @param pin analog pin to use
     */
    GroveO2(int pin);
    /**
     * GroveO2 Destructor
     */
    ~GroveO2();
    /**
     * Measures O2 from the sensor
     *
     * @return the Oxygen concentration as voltage
     */
    float voltageValue();

  private:
    mraa_aio_context m_aio;
  };
}


