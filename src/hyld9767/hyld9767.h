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

// EZ series is volts/512
#define HYLD9767_RES  512

namespace upm {
  /**
   * @brief DFRobot Loudness Sensor V2
   * @defgroup hyld9767 libupm-hyld9767
   * @ingroup dfrobot analog sound
   */

  /**
   * @library hyld9767
   * @sensor hyld9767
   * @comname DFRobot Loudness Sensor V2
   * @altname HYLD9767
   * @type sound
   * @man dfrobot
   * @web http://www.dfrobot.com/index.php?route=product/product&product_id=83
   * @con analog
   *
   * @brief API for the DFRobot Loudness Sensor V2
   *
   * This sensor returns an analog voltage proportional to the
   * loudness of the ambient environment.  It's output does not
   * correspond to a particular sound level in decibels.
   *
   * This device uses an HYLD9767 electret microphone for sound input.
   *
   * This driver was developed using the DFRobot Loudness Sensor V2
   *
   * @snippet hyld9767.cxx Interesting
   */

  class HYLD9767 {
  public:

    /**
     * HYLD9767 constructor
     *
     * @param pin Analog pin to use
     * @param aref Analog reference voltage; default is 5.0 V
     */
    HYLD9767(int pin, float aref=5.0);

    /**
     * HYLD9767 destructor
     */
    ~HYLD9767();

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


