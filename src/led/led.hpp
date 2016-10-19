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
#include "led.hpp"

namespace upm {
/**
 * @brief LED library
 * @defgroup ledlib libupm-led
 * @ingroup seeed gpio led gsk
 */

/**
 * @library ledlib
 * @sensor led
 * @comname LED
 * @altname Grove LED
 * @type led
 * @man seeed
 * @con gpio
 * @kit gsk
 *
 * @brief API for the LED
 *
 * UPM module for the LED (or other similar light-emitting diodes).
 * An LED is a p-n junction semiconductor which emits light in
 * response to voltage. The longer wire of an LED connects
 * to the positive seat (anode); the shorter wire connects to the
 * negative seat (cathode). The flat side of the bulb corresponds
 * to the cathode, while the rounded side corresponds to the anode.
 *
 * @image html led.jpg
 * @snippet groveled.cxx Interesting
 * @snippet groveled-multi.cxx Interesting
 */
class Led {
    public:
        /**
         * LED constructor
         *
         * @param gpio Pin to use
         */
        Led(int pin);
        /**
         * LED destructor
         */
        ~Led();
        /**
         * Turns the LED on or off, depending on the value.
         * If the value is positive (greater than or equal
         * to 1), the LED is turned on.  Otherwise, for 0
         * or negative values, the LED is turned off.
         *
         * @param value Tells the LED to turn on (for values >=1)
         * or off (for values <1)
         *
         * @return 0 if successful, non-zero otherwise
         */
        mraa_result_t write(int value);
        /**
         * Turns the LED off
         *
         * @return 0 if successful, non-zero otherwise
         */
        mraa_result_t off();
        /**
         * Turns the LED on
         *
         * @return 0 if successful, non-zero otherwise
         */
        mraa_result_t on();
        std::string name()
        {
            return "LED Sensor";
        }
    private:
        mraa_gpio_context m_gpio;
};
}
