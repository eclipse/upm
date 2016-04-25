/*
 * Author: Shawn Hymel
 * Copyright (c) 2015 SparkFun Electronics
 *
 * Credits to Adafruit.
 * Based on Adafruit ILI9341 library, see original license in license.txt file.
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

// Includes
#include <string>
#include <mraa/common.hpp>
#include <mraa/gpio.hpp>
#include <mraa/spi.hpp>
#include "ili9341_gfx.hpp"

// Defines
#define ILI9341_TFTWIDTH    240
#define ILI9341_TFTHEIGHT   320

#define SPI_FREQ            15000000

#define ILI9341_NOP         0x00
#define ILI9341_SWRESET     0x01
#define ILI9341_RDDID       0x04
#define ILI9341_RDDST       0x09

#define ILI9341_SLPIN       0x10
#define ILI9341_SLPOUT      0x11
#define ILI9341_PTLON       0x12
#define ILI9341_NORON       0x13

#define ILI9341_RDMODE      0x0A
#define ILI9341_RDMADCTL    0x0B
#define ILI9341_RDPIXFMT    0x0C
#define ILI9341_RDIMGFMT    0x0D
#define ILI9341_RDSELFDIAG  0x0F

#define ILI9341_INVOFF      0x20
#define ILI9341_INVON       0x21
#define ILI9341_GAMMASET    0x26
#define ILI9341_DISPOFF     0x28
#define ILI9341_DISPON      0x29

#define ILI9341_CASET       0x2A
#define ILI9341_PASET       0x2B
#define ILI9341_RAMWR       0x2C
#define ILI9341_RAMRD       0x2E

#define ILI9341_PTLAR       0x30
#define ILI9341_MADCTL      0x36
#define ILI9341_PIXFMT      0x3A

#define ILI9341_FRMCTR1     0xB1
#define ILI9341_FRMCTR2     0xB2
#define ILI9341_FRMCTR3     0xB3
#define ILI9341_INVCTR      0xB4
#define ILI9341_DFUNCTR     0xB6

#define ILI9341_PWCTR1      0xC0
#define ILI9341_PWCTR2      0xC1
#define ILI9341_PWCTR3      0xC2
#define ILI9341_PWCTR4      0xC3
#define ILI9341_PWCTR5      0xC4
#define ILI9341_VMCTR1      0xC5
#define ILI9341_VMCTR2      0xC7

#define ILI9341_RDID1       0xDA
#define ILI9341_RDID2       0xDB
#define ILI9341_RDID3       0xDC
#define ILI9341_RDID4       0xDD

#define ILI9341_GMCTRP1     0xE0
#define ILI9341_GMCTRN1     0xE1

#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04

#define HIGH                1
#define LOW                 0

#define DELAY               0x80

// Color definitions
#define ILI9341_BLACK       0x0000      /*   0,   0,   0 */
#define ILI9341_NAVY        0x000F      /*   0,   0, 128 */
#define ILI9341_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define ILI9341_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define ILI9341_MAROON      0x7800      /* 128,   0,   0 */
#define ILI9341_PURPLE      0x780F      /* 128,   0, 128 */
#define ILI9341_OLIVE       0x7BE0      /* 128, 128,   0 */
#define ILI9341_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define ILI9341_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define ILI9341_BLUE        0x001F      /*   0,   0, 255 */
#define ILI9341_GREEN       0x07E0      /*   0, 255,   0 */
#define ILI9341_CYAN        0x07FF      /*   0, 255, 255 */
#define ILI9341_RED         0xF800      /* 255,   0,   0 */
#define ILI9341_MAGENTA     0xF81F      /* 255,   0, 255 */
#define ILI9341_YELLOW      0xFFE0      /* 255, 255,   0 */
#define ILI9341_WHITE       0xFFFF      /* 255, 255, 255 */
#define ILI9341_ORANGE      0xFD20      /* 255, 165,   0 */
#define ILI9341_GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define ILI9341_PINK        0xF81F

namespace upm {
    /**
     * @brief ILI9341 LCD library
     * @defgroup ili9341 libupm-ili9341
     * @ingroup adafruit spi display
     */
    /**
     * @library ili9341
     * @sensor ili9341
     * @comname ILI9341 LCD
     * @type display
     * @man adafruit
     * @web http://www.adafruit.com/product/2090
     * @con spi
     *
     * @brief API for the ILI9342 LCd
     *
     * This module defines the interface for the ILI9341 display library
     *
     * @image html ili9341.jpg
     * @snippet ili9341.cxx Interesting
     */
    class ILI9341 : public GFX {
        public:
            /**
             * Instantiates an ILI9341 object
             *
             * @param csLCD LCD chip select pin
             * @param csSD SD card select pin
             * @param dc Data/command pin
             * @param rst Reset pin
             */
            ILI9341(uint8_t csLCD, uint8_t csSD, uint8_t dc, uint8_t rst);
            
            /**
             * Returns the name of the component
             */
            std::string name() {
                return m_name;
            }
            
            /**
             * Initializes GPIOs used to talk to the LCD
             */
            void initModule();
            
            /**
             * Configures the LCD driver chip via SPI
             */
            void configModule();
            
            /**
             * Sets the window size inside the screen where pixel data is 
             * written. Concrete implementation from GFX interface.
             *
             * @param x0 First coordinate
             * @param y0 First coordinate
             * @param x1 Second coordinate
             * @param y1 Second coordinate
             */
            void setAddrWindow(uint16_t x0, 
                               uint16_t y0, 
                               uint16_t x1, 
                               uint16_t y1);
            
            /**
             * Sends a pixel color (RGB) to the driver chip. Concrete
             * implementation from GFX interface.
             *
             * @param x Axis on the horizontal scale
             * @param y Axis on the vertical scale
             * @param color RGB (16-bit) color (R[0-4], G[5-10], B[11-15]
             */
            void drawPixel(int16_t x, int16_t y, uint16_t color);

            /**
             * Draws a vertical line using minimal SPI writes.
             *
             * @param x Axis on the horizontal scale to begin line
             * @param y Axis on the vertical scale to begin line
             * @param h Height of line in pixels
             * @param color RGB (16-bit) color (R[0-4], G[5-10], B[11-15]
             */
            void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
            
            /**
             * Draws a horizontal line using  minimal SPI writes.
             *
             * @param x Axis on the horizontal scale to begin line
             * @param y Axis on the vertical scale to begin line
             * @param w Width of line in pixels
             * @param color RGB (16-bit) color (R[0-4], G[5-10], B[11-15]
             */
            void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
            
            /**
             * Draw a filled rectangle.
             *
             * @param x Axis on the horizontal scale of upper-left corner
             * @param y Axis on the vertical scale of upper-left corner
             * @param w Width of rectangle in pixels
             * @param h Height of rectangle in pixels
             * @param color RGB (16-bit) color (R[0-4], G[5-10], B[11-15]
             */
            void fillRect(int16_t x, 
                          int16_t y, 
                          int16_t w, 
                          int16_t h,
                          uint16_t color);
                          
            /**
             * Fill the screen with a single color.
             *
             * @param color RGB (16-bit) color (R[0-4], G[5-10], B[11-15]
             */
            void fillScreen(uint16_t color);
            
            /**
             * Sets the screen to one of four 90 deg rotations.
             *
             * @param r Rotation setting: 0, 1, 2, 3
             */
            void setRotation(uint8_t r);
            
            /**
             * Invert colors on the display.
             *
             * @param i True or false to invert colors
             */
            void invertDisplay(bool i);
            
            /**
             * Pass 8-bit R, G, B values and get back 16-bit packed color.
             *
             * @param r Red color 0-31
             * @param g Green color 0-63
             * @param b blue color 0-31
             * @return 16-bit packed color (RGB) value
             */
            uint16_t color565(uint8_t r, uint8_t g, uint8_t b);
                        
            /**
             * Executes a set of commands and data.
             *
             * @param addr Pointer to the start of the commands/data section
             */
            void executeCMDList(const uint8_t *addr);
            
            /**
             * Sends a command to the display driver via SPI.
             *
             * @param c Command to be written
             */
            void writecommand(uint8_t c);
            
            /**
             * Sends data to the display driver via SPI
             *
             * @param d Data to be written
             */
            void writedata(uint8_t d);
            
            /**
             * Set LCD chip select to LOW
             */
            mraa::Result lcdCSOn();
            
            /**
             * Set LCD chip select to HIGH
             */
            mraa::Result lcdCSOff();
            
            /**
             * Set SD card chip select to LOW
             */
            mraa::Result sdCSOn();
            
            /**
             * Set SD card chip select to HIGH
             */
            mraa::Result sdCSOff();
            
            /**
             * Set data/command line to HIGH
             */
            mraa::Result dcHigh();
            
            /**
             * Set data/command line to LOW
             */
            mraa::Result dcLow();
            
            /**
             * Set reset line to HIGH
             */
            mraa::Result rstHigh();
            
            /**
             * Set reset line to LOW
             */
            mraa::Result rstLow();
            
        private:
            mraa::Spi   m_spi;
            uint8_t     m_spiBuffer[32];
            
            mraa::Gpio  m_csLCDPinCtx;
            mraa::Gpio  m_csSDPinCtx;
            mraa::Gpio  m_dcPinCtx;
            mraa::Gpio  m_rstPinCtx;
            
            std::string m_name;
    };
}

