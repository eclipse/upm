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

#include <stdint.h>
#include <sys/time.h>

#include <mraa/gpio.hpp>
#include <mraa/aio.hpp>

#define AD8232_DEFAULT_AREF  3.3

namespace upm {

  /**
   * @brief AD8232 heart rate monitor library
   * @defgroup ad8232 libupm-ad8232
   * @ingroup sparkfun gpio medical
   */

  /**
   * @library ad8232
   * @sensor ad8232
   * @comname AD8232 Heart Rate Monitor
   * @type medical
   * @man sparkfun
   * @web https://www.sparkfun.com/products/12650
   * @con gpio aio
   *
   * @brief UPM module for the AD8232 Heart Rate Monitor
   *
   * Note, this sensor must be driven at 3.3v only.
   *
   * This module will simply spit out the ADC values reported by the
   * sensor, with the intent being to send that data somewhere (via
   * serial or network port) to another piece of software running on a
   * computer that will plot the data for you, like an EKG.
   * 
   * Processing (https://www.processing.org/), is a piece of software
   * that should work, using information from the Sparkfun website.
   *
   * This example just dumps the raw data.
   *
   * @image html ad8232.jpg
   * <br><em>AD8232 Heart Rate Monitor image provided by SparkFun under
   * <a href=https://creativecommons.org/licenses/by-nc-sa/3.0/>
   * CC BY-NC-SA-3.0</a>.</em>
   *
   * @snippet ad8232.cxx Interesting
   */

  class AD8232 {
  public:

    /**
     * AD8232 constructor
     *
     * @param loPlus digital pin to use for LO+
     * @param loMinus digital pin to use LO-
     * @param output analog pin to use for reading the data
     */
    AD8232(int loPlus, int loMinus, int output, float aref=AD8232_DEFAULT_AREF);

    /**
     * AD8232 Destructor
     */
    ~AD8232();

    /**
     * return the current ADC value for the device output pin.  If an
     * LO (Leads Off) event is detected, 0 will be returned.
     *
     * @return ADC value
     */
    int value();

  private:
    mraa::Aio m_aioOUT;
    mraa::Gpio m_gpioLOPlus;
    mraa::Gpio m_gpioLOMinus;

    float m_aref;
    int m_ares;

  };
}


