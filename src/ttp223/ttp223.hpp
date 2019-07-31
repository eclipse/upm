/*
 * Author: Sarah Knepper <sarah.knepper@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <string>
#include <mraa/gpio.hpp>
#include <interfaces/iButton.hpp>

namespace upm {
/**
 * @brief TTP223 Touch Detector Sensor
 * @defgroup ttp223 libupm-ttp223
 * @ingroup seeed gpio touch
 */
/**
 * @library ttp223
 * @sensor ttp223
 * @comname Capacitive Touch Sensor
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
class TTP223 : virtual public iButton {
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
        virtual bool isPressed();

        /**
         * Installs an interrupt service routine (ISR) to be called when
         * the button is activated or deactivated.
         *
         * @param isr Pointer to a function to be called on interrupt
         * @param arg Pointer to an object to be supplied as an
         * argument to the ISR.
         */

        void installISR(mraa::Edge level, void (*isr)(void *), void *arg);
        /**
         * Uninstalls the previously installed ISR
         *
         */
        void uninstallISR();

    protected:
        std::string         m_name; //!< name of this sensor
        mraa_gpio_context   m_gpio; //!< GPIO pin
        bool                m_isrInstalled;
};

}
