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
#include <mraa/aio.h>
#include <mraa/gpio.h>
#include <mraa/spi.h>

#define HIGH                    1
#define LOW                     0

namespace upm {

/**
 * @brief FastPixel lpd8806 library
 * @defgroup lpd8806 libupm-lpd8806
 */

/**
 * @brief C++ API for LPD8806 
 *
 * The FastPixel lpd8806 is an RGB led strip controller.
 *
 * @ingroup lpd8806
 * @snippet lpd8806.cxx Interesting
 * @image html lpd8806.jpeg
 */
class LPD8806 {
    public:

        /**
         * Instanciates a LPD8806 object
         *
         * @param pixelCount number of pixels in the strip
         * @param csn chip select pin
         */
        LPD8806 (uint16_t pixelCount, uint8_t csn);

        /**
         * LPD8806 object destructor, basicaly it close SPI and the GPIO.
         */
        ~LPD8806 ();

        /**
         * @param pixelOffset pixel offset in the strip of pixel
         * @param r red led
         * @param g green led
         * @param b blue led
         */
        void setPixelColor (uint16_t pixelOffset, uint8_t r, uint8_t g, uint8_t b);

        /**
         * Write the data stored in array of pixels to the chip
         */
        void show (void);

        /**
         * Return length of the led strip
         */
        uint16_t getStripLength (void);

        /**
         * Return name of the component
         */
        std::string name()
        {
            return m_name;
        }
    private:
        std::string m_name;
        mraa_spi_context        m_spi;
        mraa_gpio_context       m_csnPinCtx;

        uint8_t*                m_pixels;
        uint8_t                 m_pixelsCount;

        uint8_t readRegister (uint8_t reg);
        void writeRegister (uint8_t reg, uint8_t data);

        /**
         * Set chip select pin LOW
         */
        mraa_result_t CSOn ();

        /**
         * Set chip select pin HIGH
         */
        mraa_result_t CSOff ();
};

}
