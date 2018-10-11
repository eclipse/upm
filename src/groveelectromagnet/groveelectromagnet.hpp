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
#include <interfaces/iElectromagnet.hpp>

#define HIGH      1
#define LOW       0

namespace upm {
  /**
   * @deprecated This library is being replaced by libupm-electromagnet
   * @brief Grove Electromagnet Library
   * @defgroup groveelectromagnet libupm-groveelectromagnet
   * @ingroup seeed gpio electric
   */

  /**
   * @deprecated This class is being replaced by Electromagnet
   * @library groveelectromagnet
   * @sensor groveelectromagnet
   * @comname Electromagnet
   * @type electric
   * @man seeed
   * @con gpio
   *
   * @brief API for the Grove Electromagnet
   *
   * The Grove Electromagnet can hold up to 1 kg (approximately 2.2 lbs)
   *
   * @image html groveelectromagnet.jpg
   * @snippet groveelectromagnet.cxx Interesting
   */
  class GroveElectromagnet : virtual public iElectromagnet {
  public:
    /**
     * Grove Electromagnet constructor
     *
     * @param pin Digital pin to use
     */
    GroveElectromagnet(int pin);
    /**
     * Grove Electromagnet destructor
     */
    ~GroveElectromagnet();
    /**
     * Turns the magnet on
     */
     virtual void on();
    /**
     * Turns the magnet off
     */
     virtual void off();

  private:
        mraa_gpio_context m_gpio;
	};
}
