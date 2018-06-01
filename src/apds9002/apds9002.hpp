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
#include <interfaces/iLight.hpp>

namespace upm {
  /**
   * @brief APDS-9002 Luminance Sensor
   * @defgroup apds9002 libupm-apds9002
   * @ingroup seeed analog light
   */

  /**
   * @library apds9002
   * @sensor apds9002
   * @comname Ambient Light Photo Sensor
   * @altname Grove Luminance Sensor
   * @type light
   * @man seeed
   * @con analog
   * @web https://www.seeedstudio.com/Grove-Luminance-Sensor-p-1941.html
   *
   * @brief UPM module for the APDS-9002 Luminance Sensor
   *
   * This sensor transforms luminous intensity to output analog values.
   * It uses an APDS-9002 ambient light sensor.
   *
   * @image html apds9002.jpg
   * @snippet apds9002.cxx Interesting
   */

  class APDS9002 : virtual public iLight {
  public:
    /**
     * APDS-9002 luminance sensor constructor
     *
     * @param pin Analog pin to use
     */
    APDS9002(int pin);
    /**
     * APDS9002 destructor
     */
    ~APDS9002();
    /**
     * Gets the luminance value from the sensor
     *
     * @return Luminance value
     */
    int value();

    /**
     * Gets the luminance value from the sensor
     *
     * @return The measured light intensity value in Lux
     */
    virtual float getLuminance();

  private:
    mraa_aio_context m_aio;
  };
}


