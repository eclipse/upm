/*
 * Author: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * Based on Adafruit SSD1351 library.
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

#include <mraa/common.hpp>
#include <mraa/gpio.hpp>
#include <mraa/spi.hpp>
#include "ssd1351_gfx.hpp"

// Display Size
#define SSD1351WIDTH 128
#define SSD1351HEIGHT 128  // Set this to 96 for 1.27"

// SSD1351 Commands
#define SSD1351_CMD_SETCOLUMN       0x15
#define SSD1351_CMD_SETROW          0x75
#define SSD1351_CMD_WRITERAM        0x5C
#define SSD1351_CMD_READRAM         0x5D
#define SSD1351_CMD_SETREMAP        0xA0
#define SSD1351_CMD_STARTLINE       0xA1
#define SSD1351_CMD_DISPLAYOFFSET   0xA2
#define SSD1351_CMD_DISPLAYALLOFF   0xA4
#define SSD1351_CMD_DISPLAYALLON    0xA5
#define SSD1351_CMD_NORMALDISPLAY   0xA6
#define SSD1351_CMD_INVERTDISPLAY   0xA7
#define SSD1351_CMD_FUNCTIONSELECT  0xAB
#define SSD1351_CMD_DISPLAYOFF      0xAE
#define SSD1351_CMD_DISPLAYON       0xAF
#define SSD1351_CMD_PRECHARGE       0xB1
#define SSD1351_CMD_DISPLAYENHANCE  0xB2
#define SSD1351_CMD_CLOCKDIV        0xB3
#define SSD1351_CMD_SETVSL          0xB4
#define SSD1351_CMD_SETGPIO         0xB5
#define SSD1351_CMD_PRECHARGE2      0xB6
#define SSD1351_CMD_SETGRAY         0xB8
#define SSD1351_CMD_USELUT          0xB9
#define SSD1351_CMD_PRECHARGELEVEL  0xBB
#define SSD1351_CMD_VCOMH           0xBE
#define SSD1351_CMD_CONTRASTABC     0xC1
#define SSD1351_CMD_CONTRASTMASTER  0xC7
#define SSD1351_CMD_MUXRATIO        0xCA
#define SSD1351_CMD_COMMANDLOCK     0xFD
#define SSD1351_CMD_HORIZSCROLL     0x96
#define SSD1351_CMD_STOPSCROLL      0x9E
#define SSD1351_CMD_STARTSCROLL     0x9F

#define HIGH                1
#define LOW                 0

// Number of blocks for SPI transfer of buffer
#define BLOCKS              8

namespace upm {
/**
 * @brief SSD1351 OLED library
 * @defgroup ssd1351 libupm-ssd1351
 * @ingroup adafruit spi display
 */
/**
 * @library ssd1351
 * @sensor ssd1351
 * @comname SSD1351 OLED
 * @type display
 * @man adafruit
 * @web http://www.adafruit.com/products/1431
 * @con spi
 *
 * @brief API for SSD1351 OLED displays
 *
 * This module defines the interface for the SSD1351 display library. It was
 * tested with the Adafruit 1.5" OLED Display, but should work with any SSD1351
 * display running in SPI mode.
 *
 * On the Intel Edison don't forget to disable SPI Power Management (PM) for
 * this driver to work, you can find more details on this topic here:
 * http://iotdk.intel.com/docs/master/mraa/edison.html
 *
 * @image html ssd1351.jpg
 * @snippet ssd1351.cxx Interesting
 */
class SSD1351 : public GFX{
    public:
        /**
         * Instantiates an SSD1351 object
         *
         * @param oc LCD chip select pin
         * @param dc Data/command pin
         * @param rst Reset pin
         */
        SSD1351 (uint8_t oc, uint8_t dc, uint8_t rst);

        /**
         * SSD1351 object destructor
         */
        ~SSD1351();

        /**
         * Returns the name of the component
         */
        std::string name()
        {
            return m_name;
        }

        /**
         * Sends a command to an SPI bus
         *
         * @param value Command
         */
        void writeCommand (uint8_t value);

        /**
         * Sends data to an SPI bus
         *
         * @param value Data
         */
        void writeData (uint8_t value);
        /**
         * Sends a pixel color (RGB) to the display buffer or chip
         *
         * @param x Axis on the horizontal scale
         * @param y Axis on the vertical scale
         * @param color RGB (16-bit) color (R[0-4], G[5-10], B[11-15])
         */
        void drawPixel (int16_t x, int16_t y, uint16_t color);

        /**
         * Copies the buffer to the chip via the SPI bus
         */
        void refresh ();

        /**
         * Set OLED chip select LOW
         */
        void ocLow ();

        /**
         * Set OLED chip select HIGH
         */
        void ocHigh ();

        /**
         * Data select LOW
         */
        void dcLow ();

        /**
         * Data select HIGH
         */
        void dcHigh ();

        /**
         * Use memory mapped (buffered) writes to the display
         *
         * @param var true for yes (default), false for no
         */
        void useMemoryMap (bool var);
    private:
        mraa::Spi       m_spi;
        uint8_t         m_map[SSD1351HEIGHT * SSD1351WIDTH * 2]; /**< Screen buffer */
        bool            m_usemap;

        mraa::Gpio      m_oc;
        mraa::Gpio      m_dc;
        mraa::Gpio      m_rst;

        std::string     m_name;
};
}
