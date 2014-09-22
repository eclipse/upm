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
 * @brief Olimex LoL array library
 * @defgroup lol libupm-lol
 */

/**
 * @brief C++ API for Olimex LoL array
 *
 * This file defines the LoL API and implemntation for a simple framebuffer
 *
 * @ingroup lol
 * @snippet lol-example.cxx Interesting
 */
class LoL {
    public:
        /**
         * Instantiates a LoL object
         * singleton
         */
        LoL();

        /**
         * LoL object destructor
         */
        ~LoL();

        /**
         * Get framebuffer pointer
         * @return 0 on success
         */
        unsigned char *getFramebuffer();

        /**
         * Gets pixel at coordinates
         * @param x coordinate x
         * @param y coordinate y
         * @return 1 if pixel is on, 0 if off, -1 on error
         */
        unsigned char getPixel(int x, int y);

        /**
         * sets pixel at coordinates
         * @param x coordinate x
         * @param y coordinate y
         * @param pixel 0 is off, 1 is on
         * @return 0 on success, -1 on error
         */
        unsigned char setPixel(int x, int y, unsigned char pixel);

        /**
         * Get framebuffer pointer
         * @return 1 if pixel is on, 0 if off, -1 on error
         */
        unsigned char getBit(int x, int y);


    private:
        mraa_gpio_context    m_LoLCtx[14];
        unsigned char framebuffer[LOL_X*LOL_Y];
        pthread_t        drawer_thread;
};
};

