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
 * @brief Button library
 * @defgroup buttonlib libupm-button
 * @ingroup seeed gpio button gsk
 */

/**
 * @library buttonlib
 * @sensor button
 * @comname Button
 * @altname Grove Button
 * @type button
 * @man seeed
 * @con gpio
 * @kit gsk
 *
 * @brief API for the Button
 *
 * Basic UPM module for the button sensor
 *
 * @image html button.jpg
 * @snippet button.cxx Interesting
 */
class Button{
    public:
        /**
         * button constructor
         *
         * @param gpio Pin to use
         */
        Button(unsigned int pin);
        /**
         * button destructor
         */
        ~Button();
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
         * Installs an interrupt service routine (ISR) to be called when
         * the button is activated or deactivated.
         *
         * @param fptr Pointer to a function to be called on interrupt
         * @param arg Pointer to an object to be supplied as an
         * argument to the ISR.
         */
#if defined(SWIGJAVA) || defined(JAVACALLBACK)
        void installISR(mraa::Edge level, jobject runnable);
#else
        void installISR(mraa::Edge level, void (*isr)(void *), void *arg);
#endif
        /**
         * Uninstalls the previously installed ISR
         *
         */
        void uninstallISR();

    private:
#if defined(SWIGJAVA) || defined(JAVACALLBACK)
        void installISR(mraa::Edge level, void (*isr)(void *), void *arg);
#endif
        bool m_isrInstalled;
        std::string m_name;
        mraa_gpio_context m_gpio;
};
}
