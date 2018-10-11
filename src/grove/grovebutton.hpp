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
#include <interfaces/iButton.hpp>

namespace upm {

/**
 * @deprecated This class is being replaced by the upm-button library and the
 * Button class.
 * @library grove
 * @sensor grovebutton
 * @comname Button
 * @altname Grove Touch Sensor
 * @type button touch
 * @man seeed
 * @con gpio
 * @kit gsk
 *
 * @brief API for the Grove Button
 *
 * Basic UPM module for the Grove button
 *
 * @image html grovebutton.jpg
 * @snippet grove-grovebutton.cxx Interesting
 */
class GroveButton: public Grove, virtual public iButton {
    public:
        /**
         * Grove button constructor
         *
         * @param pin Pin to use
         */
        GroveButton(unsigned int pin);
        /**
         * Grove button destructor
         */
        ~GroveButton();
        /**
         * Gets the name of the sensor
         *
         * @return Name of this sensor
         */
        std::string name();
        /**
         * Gets the value from the GPIO pin
         *
         * @return Value from the GPIO pin
         */
        int value();

        /**
         * Gets the current button press state.
         *
         * @returns Button state
         */
         virtual bool isPressed();

        /**
         * Installs an interrupt service routine (ISR) to be called when
         * the button is activated or deactivated.
         *
         * @param fptr Pointer to a function to be called on interrupt
         * @param arg Pointer to an object to be supplied as an
         * argument to the ISR.
         */
        void installISR(mraa::Edge level, void (*isr)(void *), void *arg);

        /**
         * Uninstalls the previously installed ISR
         *
         */
        void uninstallISR();

    private:
        bool m_isrInstalled;
        std::string m_name;
        mraa_gpio_context m_gpio;
};
}
