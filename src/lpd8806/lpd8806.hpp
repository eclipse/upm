/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
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
#include <mraa/aio.hpp>

#include <mraa/gpio.hpp>

#include <mraa/spi.hpp>

#define HIGH                    1
#define LOW                     0

namespace upm {

/**
 * @brief FastPixel LPD8806 library
 * @defgroup lpd8806 libupm-lpd8806
 * @ingroup adafruit spi led
 */

/**
 * @library lpd8806
 * @sensor lpd8806
 * @comname LPD8806 RGB LED Strip Controller
 * @type led
 * @man adafruit
 * @con spi
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
         * @param pixelCount Number of pixels in the strip
         * @param csn Chip select pin
         */
        LPD8806 (uint16_t pixelCount, uint8_t csn);

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
