/*
 * Authors: Brendan Le Foll <brendan.le.foll@intel.com>
 *          Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 *          Sarah Knepper <sarah.knepper@intel.com>
 * Copyright (c) 2014 - 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string>
#include "led.h"

namespace upm {
/**
 * @brief LED Library
 * @defgroup ledlib libupm-led
 * @ingroup seeed gpio led gsk
 */

/**
 * @library ledlib
 * @sensor led
 * @comname Light-emitting Diode (LED)
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
 * @snippet led.cxx Interesting
 */
class Led {
    public:
        /**
         * LED constructor
         *
         * @param pin Pin to use
         */
        Led(int pin);
        /**
         * LED constructor
         *
         * @param name Linux gpioled device name
         */
        Led(std::string name);
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
        upm_result_t write(int value);
        /**
         * Turns the LED off
         *
         * @return 0 if successful, non-zero otherwise
         */
        upm_result_t off();
        /**
         * Turns the LED on
         *
         * @return 0 if successful, non-zero otherwise
         */
        upm_result_t on();
        std::string name()
        {
            return "LED Sensor";
        }
    private:
        led_context m_led;
};
}
