/*
 * Author: Alexander Komarov <alexander.komarov@intel.com>
 * Copyright (c) 2014 Intel Corporation.
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
#include <mraa/gpio.h>
#include <mraa/aio.h>

namespace upm {

/**
 * @brief Analog Joystick library
 * @defgroup joystick libupm-joystick
 * @ingroup generic analog ainput
 */

/**
 * @library joystick
 * @sensor joystick
 * @comname Analog Joystick
 * @type ainput
 * @man generic
 * @con analog
 *
 * @brief C++ API for Elecfreaks Joystick v 1.2-1.4 breakout
 *
 * This file defines the Joystick API and implemntation for X, Y
 * button could be treated as normal GPIO, this enables easier
 * interrupt support
 *
 * @image html joystick12.jpg
 * @snippet joystick12-example.cxx Interesting
 */
class Joystick12 {
    public:
        /**
         * Instantiates a Joystick object
         *
         * @param pinX analog pin where X input is connected
         * @param pinY analog pin where Y input is connected
         */
        Joystick12(int pinX, int pinY);

        /**
         * Joystick object destructor
         */
        ~Joystick12();

        /**
         * Get X input
         * @return float X value, range from -1 to 1. 0 is mid
         */
        float getXInput();

        /**
         * Get Y input
         *
         * @return float Y value, range from -1 to 1. 0 is mid
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

