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
#include <mraa/initio.hpp>
#include <mraa/gpio.hpp>
#include <interfaces/iButton.hpp>

namespace upm {

/**
 * @brief Button/Switch Library
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
class Button : virtual public iButton {
    public:
        /**
         * button constructor
         *
         * @param pin Pin to use
         */
        Button(unsigned int pin);
        /**
         * Instantiates Button/Switch Library based on a given string.
         *
         * @param initStr string containing specific information for Button initialization.
         */
        Button(std::string initStr);
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
        mraa::MraaIo mraaIo;
};
}
