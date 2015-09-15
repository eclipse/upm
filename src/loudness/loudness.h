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
#include <mraa/aio.hpp>

namespace upm {
  /**
   * @brief Generic loudness sensors
   * @defgroup loudness libupm-loudness
   * @ingroup dfrobot seeed analog sound
   */

  /**
   * @library loudness
   * @sensor loudness
   * @comname Loudness Sensor
   * @altname Grove loudness hyld9767
   * @type sound
   * @man dfrobot seeed
   * @web http://www.dfrobot.com/index.php?route=product/product&product_id=83
   * @con analog
   *
   * @brief API for the Loudness Sensor
   *
   * This sensor family returns an analog voltage proportional to the
   * loudness of the ambient environment.  It's output does not
   * correspond to a particular sound level in decibels.  The higher
   * the output voltage, the louder the ambient noise level.
   *
   * This device uses an electret microphone for sound input.
   *
   * This driver was developed using the DFRobot Loudness Sensor V2
   * and the Grove Loudness sensor.
   *
   * @image html groveloudness.jpg
   * @snippet loudness.cxx Interesting
   */

  class Loudness {
  public:

    /**
     * Loudness constructor
     *
     * @param pin Analog pin to use
     * @param aref Analog reference voltage; default is 5.0 V
     */
    Loudness(int pin, float aref=5.0);

    /**
     * Loudness destructor
     */
    ~Loudness();

    /**
     * Returns the voltage detected on the analog pin
     *
     * @return The detected voltage
     */
    float loudness();

  protected:
    mraa::Aio m_aio;

  private:
    float m_aref;
    // ADC resolution
    int m_aRes;
  };
}


