/*
 * Author: Alexander Komarov <alexander.komarov@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <string>
#include <mraa/gpio.h>
#include <mraa/aio.h>

namespace upm {

/**
 * @brief Analog Joystick Library
 * @defgroup joystick12 libupm-joystick12
 * @ingroup generic analog ainput robok
 */

/**
 * @library joystick12
 * @sensor joystick12
 * @comname 2-axis Analog Joystick
 * @type ainput
 * @man generic
 * @con analog
 * @kit robok
 *
 * @brief API for the ElecFreaks* Joystick v 1.2-1.4 Breakout
 *
 * This module defines the Joystick API, and implementation for the X and Y
 * buttons could be treated as normal GPIO - this enables easier
 * interrupt support. This driver should be compatible with any
 * 2-axis analog joystick.
 *
 * @image html joystick12.jpg
 * @snippet joystick12.cxx Interesting
 */
class Joystick12 {
    public:
        /**
         * Instantiates a Joystick object
         *
         * @param pinX Analog pin where the X input is connected
         * @param pinY Analog pin where the Y input is connected
         */
        Joystick12(int pinX, int pinY);

        /**
         * Joystick object destructor
         */
        ~Joystick12();

        /**
         * Gets the X input
         * @return float X value, ranging from -1 to 1; 0 is mid
         */
        float getXInput();

        /**
         * Gets the Y input
         *
         * @return float Y value, ranging from -1 to 1; 0 is mid
         */
        float getYInput();

    private:
        mraa_aio_context    m_joystickCtxX;
        mraa_aio_context    m_joystickCtxY;

        static const int X_left;
        static const int X_center;
        static const int X_right;
        static const int Y_left;
        static const int Y_center;
        static const int Y_right;
};
};

