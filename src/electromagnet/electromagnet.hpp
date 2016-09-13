/*
 * Author: Zion Orent <sorent@ics.com>
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

#include <mraa/gpio.h>

#define HIGH      1
#define LOW       0

namespace upm {
  /**
   * @brief  Electromagnet library
   * @defgroup electromagnet libupm-electromagnet
   * @ingroup seeed gpio electric
   */

  /**
   * @library electromagnet
   * @sensor electromagnet
   * @comname  Electromagnet
   * @type electric
   * @man seeed
   * @con gpio
   *
   * @brief API for the  Electromagnet
   * 
   * The  Electromagnet can hold up to 1 kg (approximately 2.2 lbs)
   *
   * @image html electromagnet.jpg
   * @snippet electromagnet.cxx Interesting
   */
  class Electromagnet {
  public:
    /**
     *  Electromagnet constructor
     *
     * @param pin Digital pin to use
     */
    Electromagnet(int pin);
    /**
     *  Electromagnet destructor
     */
    ~Electromagnet();
    /**
     * Turns the magnet on
     */
     void on();
    /**
     * Turns the magnet off
     */
     void off();

  private:
        mraa_gpio_context m_gpio;
	};
}
