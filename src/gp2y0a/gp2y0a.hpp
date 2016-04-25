/*
 * Author: Jon Trulson <jtrulson@ics.com>
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

#include <iostream>
#include <string>
#include <mraa/aio.h>

namespace upm {
  /**
   * @brief GP2Y0A-based IR Proximity Sensor library
   * @defgroup gp2y0a libupm-gp2y0a
   * @ingroup seeed analog light
   */

  /**
   * @library gp2y0a
   * @sensor gp2y0a
   * @comname GP2Y0A IR Proximity Sensor
   * @altname Grove 80cm IR Proximity Sensor
   * @type light
   * @man seeed
   * @con analog
   *
   * @brief API for the GP2Y0A family of IR Proximity Sensors
   *
   * Sensors of this family return an analog voltage corresponding to the distance 
   * of an object from the sensor. The voltage is lower when objects
   * are far away; the voltage increases as objects get closer
   * to the sensor.  
   *
   * @image html gp2y0a.jpg
   * @snippet gp2y0a.cxx Interesting
   */
  class GP2Y0A {
  public:

    /**
     * GP2Y0A sensor constructor
     *
     * @param pin Analog pin to use
     */
    GP2Y0A(int pin);

    /**
     * GP2Y0A destructor
     */
    ~GP2Y0A();

    /**
     * Gets an average voltage value from the sensor
     *
     * @param aref Reference voltage in use (usually 5.0V or 3.3V)
     * @param samples Number of samples to average over
     * @return Average voltage reading
     */
    float value(float aref, uint8_t samples);

  private:
    mraa_aio_context m_aio;
    // ADC resolution
    int m_aRes;
  };
}


