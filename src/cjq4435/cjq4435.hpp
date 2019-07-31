/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015-2017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include "mraa/initio.hpp"
#include "cjq4435.h"

namespace upm {
    /**
     * @brief CJQ4435 MOSFET
     * @defgroup cjq4435 libupm-cjq4435
     * @ingroup seeed gpio pwm electric robok
     */

    /**
     * @library cjq4435
     * @sensor cjq4435
     * @comname MOSFET
     * @altname CJQ4435
     * @type electric
     * @man seeed
     * @con gpio pwm
     * @kit robok
     *
     * @brief API for the CJQ4435 MOSFET
     *
     * UPM module for the CJQ4435 MOSFET. It was developed using the
     * Grove MOSFET module, but could be used with any MOSFET.  A
     * MOSFET is like a switch, but it can switch much faster than a
     * mechanical relay.  Here, we implement support via MRAA pulse
     * width modulation (PWM) functions.  Note: available periods vary
     * depending on the capabilities of your platform.
     *
     * @image html cjq4435.jpg
     * @snippet cjq4435.cxx Interesting
     */
    class CJQ4435 {
    public:
        /**
         * CJQ4435 constructor
         *
         * @param pin Digital pin to use; this pin must be PWM-capable
         */
        CJQ4435(int pin);

        /**
         * Instantiates CJQ4435 MOSFET based on a given string.
         *
         * @param initStr string containing specific information for CJQ4435 MOSFET.
         */
        CJQ4435(std::string initStr);

        /**
         * CJQ4435 destructor
         */
        ~CJQ4435();

        /**
         * Sets a period in microseconds
         *
         * @param us Period in microseconds
         */
        void setPeriodUS(int us);

        /**
         * Sets a period in milliseconds
         *
         * @param ms Period in milliseconds
         */
        void setPeriodMS(int ms);

        /**
         * Sets a period in seconds
         *
         * @param seconds Period in seconds
         */
        void setPeriodSeconds(float seconds);

        /**
         * Enables output
         *
         * @param enable Enables PWM output if true, disables otherwise
         */
        void enable(bool enable);

        /**
         * Sets a duty cycle. Duty cycle is a floating-point number
         * between 0.0 (always off) and 1.0 (always on). It represents a
         * proportion of time, per period, during which the output is
         * driven high.
         *
         * @param dutyCycle Duty cycle to use
         */
        void setDutyCycle(float dutyCycle);

        /**
         * Shortcut to turn the output to continuous on (high)
         */
        void on();

        /**
         * Shortcut to turn the output to continuous off (low)
         */
        void off();

    protected:
        cjq4435_context m_cjq4435;
        mraa::MraaIo mraaIo;

    private:
        /* Disable implicit copy and assignment operators */
        CJQ4435(const CJQ4435&) = delete;
        CJQ4435 &operator=(const CJQ4435&) = delete;
    };
}


