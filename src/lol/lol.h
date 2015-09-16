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
#include <pthread.h>

namespace upm {

#define LOL_X 14
#define LOL_Y 9

/**
 * @brief Olimex LoL Array library
 * @defgroup lol libupm-lol
 * @ingroup adafruit gpio display
 */

/**
 * @library lol
 * @sensor lol
 * @comname Olimex LoL Array
 * @type display
 * @man adafruit
 * @con gpio
 *
 * @brief API for the Olimex LoL array
 *
 * This module defines the LoL API and implementation for a simple framebuffer.
 *
 * @image html lolshield.jpg
 * @snippet lol-example.cxx Interesting
 */
class LoL {
    public:
        /**
         * Instantiates an LoL object
         * singleton
         */
        LoL();

        /**
         * LoL object destructor
         */
        ~LoL();

        /**
         * Gets a framebuffer pointer
         * @return 0 if successful
         */
        unsigned char *getFramebuffer();

        /**
         * Gets a pixel at specified coordinates
         * @param x Coordinate x
         * @param y Coordinate y
         * @return 1 if the pixel is on, 0 if off, -1 on error
         */
        unsigned char getPixel(int x, int y);

        /**
         * Sets a pixel at specified coordinates
         * @param x Coordinate x
         * @param y Coordinate y
         * @param pixel 0 is off, 1 is on
         * @return 0 if successful, -1 on error
         */
        unsigned char setPixel(int x, int y, unsigned char pixel);

    private:
        mraa_gpio_context m_LoLCtx[14];
        unsigned char framebuffer[LOL_X*LOL_Y];
        pthread_t drawer_thread;
};
};

