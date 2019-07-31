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
#include <pthread.h>

namespace upm {

#define LOL_X 14
#define LOL_Y 9

/**
 * @brief Olimex LoL Shield
 * @defgroup lol libupm-lol
 * @ingroup adafruit gpio display
 */

/**
 * @library lol
 * @sensor lol
 * @comname Lots of LEDs (LoL) Array Rev A
 * @type display
 * @man adafruit
 * @con gpio
 *
 * @brief API for the Olimex LoL array
 *
 * This module defines the LoL API and implementation for a simple framebuffer.
 *
 * @image html lolshield.jpg
 * @snippet lol.cxx Interesting
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
         * @return true if the pixel is on, false if off
         * @throws std::invalid_argument if pixel is out of bounds
         */
        bool getPixel(int x, int y);

        /**
         * Sets a pixel at specified coordinates
         * @param x Coordinate x
         * @param y Coordinate y
         * @param pixel false is off, true is on
         * @throws std::invalid_argument if pixel is out of bounds
         */
        void setPixel(int x, int y, bool pixel);

    private:
        mraa_gpio_context m_LoLCtx[14];
        unsigned char framebuffer[LOL_X*LOL_Y];
        pthread_t drawer_thread;
};
};

