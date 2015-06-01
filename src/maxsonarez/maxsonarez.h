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
   * @brief UPM library for the MaxSonar-EZ family of ultrasonic rangers
   * @defgroup maxsonarez libupm-maxsonarez
   * @ingroup seeed analog sound
   */

  /**
   * @library maxsonarez
   * @sensor maxsonarez
   * @comname LV-MaxSonar-EZ family of ultrasonic rangers
   * @altname EZ1, EZ2, EZ3, EZ4
   * @type sound
   * @man sparkfun
   * @con analog
   *
   * @brief API for the LV-MaxSonar-EZ family of ultrasonic rangers
   *
   * This sensor family returns an analog voltage corresponding to the
   * distance of an object from the sensor in inches.  It has a
   * resolution of about 9.7 millivolts per inch with a 5.0 AREF.  The
   * differences between the various versions (EZ1, EZ2, etc) are
   * related to the narrowness of the beam angle.
   *
   * This class supports these sensors with an analog input only.
   *
   * This driver was developed using an LV-MaxSonar-EZ3.
   *
   * @snippet maxsonarez.cxx Interesting
   */

  class MAXSONAREZ {
  public:

    /**
     * MAXSONAREZ sensor constructor
     *
     * @param pin analog pin to use
     * @param aref the analog reference voltage, default: 5.0
     */
    MAXSONAREZ(int pin, float aref=5.0);

    /**
     * MAXSONAREZ Destructor
     */
    ~MAXSONAREZ();

    /**
     * get the distance to the object in inches
     *
     * @return the distance to the object in inches
     */
    int inches();

  private:
    mraa_aio_context m_aio;
    float m_aref;
    // ADC resolution
    int m_aRes;
    // computed Volts per Inch
    float m_vI;
  };
}


