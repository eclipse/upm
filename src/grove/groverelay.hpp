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
#include "grovebase.hpp"

namespace upm {

/**
 * @library grove
 * @sensor groverelay
 * @comname Grove Relay
 * @type relay
 * @man seeed
 * @con gpio
 * @kit gsk eak hak
 *
 * @brief API for the Grove Relay
 *
 * UPM module for the Grove relay switch. The Grove relay is a
 * digital normally-open switch that uses low voltage or current to
 * control a higher voltage and/or higher current.  When closed,
 * the indicator LED lights up and current is allowed to flow.
 *
 * @image html groverelay.jpg
 * @snippet groverelay.cxx Interesting
 */
class GroveRelay: public Grove {
    public:
        /**
         * Grove relay constructor
         *
         * @param gpio Pin to use
         */
        GroveRelay(unsigned int pin);
        /**
         * Grove relay destructor
         */
        ~GroveRelay();
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
    private:
        mraa_gpio_context m_gpio;
};
}
