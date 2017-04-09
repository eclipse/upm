/*
 * Authors: Brendan Le Foll <brendan.le.foll@intel.com>
 *          Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 *          Sarah Knepper <sarah.knepper@intel.com>
 * Copyright (c) 2014 - 2016 Intel Corporation.
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
#include <mraa/gpio.hpp>

namespace upm {
 /**
  * @brief Relay library
  * @defgroup relaylib libupm-relay
  * @ingroup seeed gpio relay gsk eak hak
  */

 /**
  * @library relaylib
  * @sensor relay
  * @comname Relay
  * @altname Grove Relay
  * @type relay
  * @man seeed
  * @con gpio
  * @kit gsk eak hak
  *
  * @brief API for the Relay
  *
  * UPM module for the relay switch. The relay is a digital normally-open
  * switch that uses low voltage or current to control a higher voltage and/or
  * higher current.  When closed, the indicator LED (if present) lights up and
  * current is allowed to flow.
  *
  * @image html relay.jpg
  * @snippet relay.cxx Interesting
  */

  class Relay{
    public:
        /**
         * relay constructor
         *
         * @param gpio Pin to use
         */
        Relay(unsigned int pin);
        /**
         * relay destructor
         */
        ~Relay();
        /**
         * Sets the relay switch to on (closed). This allows current
         * to flow and lights up the indicator LED.
         *
         * @return 0 if successful, non-zero otherwise
         */
        mraa_result_t on();
        /**
         * Sets the relay switch to off (open). This stops current
         * from flowing and the indicator LED is not lit.
         *
         * @return 0 if successful, non-zero otherwise
         */
        mraa_result_t off();
        /**
         * Defines whether the relay switch is closed.
         *
         * @return True if the switch is on (closed), false otherwise
         */
        bool isOn();
        /**
         * Defines whether the relay switch is open.
         *
         * @return True if the switch is off (open), false otherwise
         */
        bool isOff();
        std::string name(){ return "Relay Switch";}
    private:
        mraa_gpio_context m_gpio;
  };
}
