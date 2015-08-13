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

#include <mraa/gpio.h>

namespace upm {
    /**
     * @brief SX6119-based FM Receiver library
     * @defgroup sx6119 libupm-sx6119
     * @ingroup seeed gpio sound
     */

    /**
     * @sensor sx6119
     * @library sx6119
     * @comname Grove FM Receiver
     * @altname SX6119 FM Receiver
     * @type sound
     * @man seeed
     * @con gpio
     *
     * @brief API support for the SX6119-based Grove FM Receiver
     *
     * This class implements support for the SX6119 FM Receiver. There are
     * two digital pins: one that toggles power on/off, and the other that
     * does a seek to the next station.
     *
     * @image html sx6119.jpg
     * @snippet sx6119.cxx Interesting
     */
  class SX6119 {
  public:

    /**
     * SX6119 constructor
     *
     * @param powerPin Pin to use for recording
     * @param seekPin Pin to use for seeking to the next station
     */
    SX6119(int powerPin, int seekPin);

    /**
     * SX6119 destructor
     */
    ~SX6119();

    /**
     * Toggles the device power on or off
     *
     */
    void togglePower();

    /**
     * Seeks to the next station
     *
     */
    void seek();

  private:
    mraa_gpio_context m_gpioPower;
    mraa_gpio_context m_gpioSeek;
  };
}


