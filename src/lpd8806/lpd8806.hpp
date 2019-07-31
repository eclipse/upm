/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
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
#include <mraa/aio.hpp>

#include <mraa/gpio.hpp>

#include <mraa/spi.hpp>

#define HIGH                    1
#define LOW                     0

namespace upm {

/**
 * @brief FastPixel LPD8806 LED Strip
 * @defgroup lpd8806 libupm-lpd8806
 * @ingroup adafruit spi led
 */

/**
 * @library lpd8806
 * @sensor lpd8806
 * @comname Digital RGB LED Strip Controller
 * @type led
 * @man adafruit
 * @con spi
 * @web https://www.adafruit.com/product/306
 *
 * @brief API for the LPD8806 RGB LED Strip Controller
 *
 * FastPixel* LPD8806 is an RGB LED strip controller.
 *
 * @image html lpd8806.jpg
 * @snippet lpd8806.cxx Interesting
 */
class LPD8806 {
    public:

        /**
         * Instantiates an LPD8806 object
         *
         * @param bus SPI bus to use
         * @param csn Chip select pin
         * @param pixelCount Number of pixels in the strip
         */
        LPD8806 (int bus, int csn, uint16_t pixelCount);

        /**
         * LPD8806 object destructor; basically, it frees the allocated
         * pixel buffer.
         */
        ~LPD8806 ();

        /**
         * @param pixelOffset Pixel offset in the strip of the pixel
         * @param r Red LED
         * @param g Green LED
         * @param b Blue LED
         */
        void setPixelColor (uint16_t pixelOffset, uint8_t r, uint8_t g, uint8_t b);

        /**
         * Writes the data stored in the array of pixels to the chip
         */
        void show (void);

        /**
         * Returns the length of the LED strip
         */
        uint16_t getStripLength (void);

        /**
         * Returns the name of the component
         */
        std::string name()
        {
            return m_name;
        }
    private:
        /* Disable implicit copy and assignment operators */
        LPD8806(const LPD8806&) = delete;
        LPD8806 &operator=(const LPD8806&) = delete;

        std::string m_name;
        mraa::Spi        m_spi;
        mraa::Gpio       m_csnPinCtx;

        uint8_t*                m_pixels;
        uint8_t                 m_pixelsCount;

        uint8_t readRegister (uint8_t reg);
        void writeRegister (uint8_t reg, uint8_t data);

        /**
         * Sets the chip select pin to LOW
         */
        mraa::Result CSOn ();

        /**
         * Sets the chip select pin to HIGH
         */
        mraa::Result CSOff ();
};

}
