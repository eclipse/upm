/*
 * Author: Sarah Knepper <sarah.knepper@intel.com>
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

#include <string>
#include <mraa/gpio.hpp>

#if defined(SWIGJAVA) || defined(JAVACALLBACK)
#include "../IsrCallback.h"
#endif

namespace upm {
/**
 * @brief TTP223 Touch Detector Sensor library
 * @defgroup ttp223 libupm-ttp223
 * @ingroup seeed gpio touch
 */
/**
 * @library ttp223
 * @sensor ttp223
 * @comname TTP223 Touch Sensor
 * @altname Grove Touch Sensor
 * @type touch
 * @man seeed
 * @web http://www.seeedstudio.com/depot/Grove-Touch-Sensor-p-747.html
 * @con gpio
 *
 * @brief API for the TTP223 Touch Sensor
 * 
 *   This touch sensor detects when a finger is near the metallic pad
 *   by the change in capacitance. It can replace a more traditional push
 *   button. The touch sensor can still function when placed under a 
 *   non-metallic surface like glass or plastic.
 *
 * @image html ttp223.jpg
 * @snippet ttp223.cxx Interesting
 */
class TTP223 {
    public:
        /**
         * TTP223 constructor
         *
         * @param pin GPIO pin where the sensor is connected
         */
        TTP223(unsigned int pin);

        /**
         * TTP223 destructor
         */
        ~TTP223();

        /**
         * Returns the name of this sensor
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
         * Determines whether the touch sensor is being touched
         *
         * @return True if touched, false otherwise
         */
        bool isPressed();

        /**
         * Installs an interrupt service routine (ISR) to be called when
         * the button is activated or deactivated.
         *
         * @param fptr Pointer to a function to be called on interrupt
         * @param arg Pointer to an object to be supplied as an
         * argument to the ISR.
         */
#if defined(SWIGJAVA) || defined(JAVACALLBACK)
        void installISR(mraa::Edge level, IsrCallback *cb);
#else
        void installISR(mraa::Edge level, void (*isr)(void *), void *arg);
#endif
        /**
         * Uninstalls the previously installed ISR
         *
         */
        void uninstallISR();

    protected:
#if defined(SWIGJAVA) || defined(JAVACALLBACK)
        void installISR(mraa::Edge level, void (*isr)(void *), void *arg);
#endif
        std::string         m_name; //!< name of this sensor
        mraa_gpio_context   m_gpio; //!< GPIO pin
        bool                m_isrInstalled;
};

}
