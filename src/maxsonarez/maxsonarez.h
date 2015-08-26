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

// EZ series is volts/512
#define MAXSONAREZ_RES  512

namespace upm {
  /**
   * @brief MaxSonar-EZ Family of Ultrasonic Rangers library
   * @defgroup maxsonarez libupm-maxsonarez
   * @ingroup seeed analog sound
   */

  /**
   * @library maxsonarez
   * @sensor maxsonarez
   * @comname LV-MaxSonar-EZ Ultrasonic Ranger
   * @altname EZ1, EZ2, EZ3, EZ4
   * @type sound
   * @man sparkfun
   * @web https://www.sparkfun.com/products/8502
   * @con analog
   *
   * @brief API for the LV-MaxSonar-EZ Family of Ultrasonic Rangers
   *
   * Sensors of this family return an analog voltage corresponding to the
   * distance of an object from the sensor in inches. They have a
   * resolution of about 9.7 millivolts per inch with an analog reference voltage of 5.0 V. The
   * differences between various versions (EZ1, EZ2, etc.) are
   * related to the narrowness of the beam angle.
   *
   * This class supports these sensors with an analog input only.
   *
   * This driver was developed using an LV-MaxSonar-EZ3 ultrasonic ranger.
   *
   * @image html maxsonarez.jpg
   * <br><em>LV-MaxSonar-EZ Ultrasonic Ranger image provided by SparkFun* under
   * <a href=https://creativecommons.org/licenses/by-nc-sa/3.0/>
   * CC BY-NC-SA-3.0</a>.</em>
   *
   * @snippet maxsonarez.cxx Interesting
   */

  class MAXSONAREZ {
  public:

    /**
     * MAXSONAREZ constructor
     *
     * @param pin Analog pin to use
     * @param aref Analog reference voltage; default is 5.0 V
     */
    MAXSONAREZ(int pin, float aref=5.0);

    /**
     * MAXSONAREZ destructor
     */
    ~MAXSONAREZ();

    /**
     * Gets the distance to the object in inches
     *
     * @return Distance to the object in inches
     */
    int inches();

  private:
    mraa_aio_context m_aio;
    float m_aref;
    // ADC resolution
    int m_aRes;
    // computed volts per inch
    float m_vI;
  };
}


