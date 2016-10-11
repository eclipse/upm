/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * Based on UPM C++ drivers originally developed by:
 * Author: Daniel Mosquera
 * Copyright (c) 2013 Daniel Mosquera
 *
 * Author: Thomas Ingleby <thomas.c.ingleby@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Contributions: Sergey Kiselev <sergey.kiselev@intel.com>
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
#include "lcm1602.h"

namespace upm
{
/**
 * @brief LCM1602 Display library
 * @defgroup lcm1602 libupm-lcm1602
 * @ingroup dfrobot sainsmart seeed sparkfun adafruit i2c gpio display
 */

/**
 * @library lcm1602
 * @sensor lcm1602
 * @comname LCM1602 Display
 * @type display
 * @man adafruit sparkfun seeed
 * @web https://www.adafruit.com/datasheets/TC1602A-01T.pdf
 * @con i2c gpio
 *
 * @brief API for the LCM1602 I2C controller for HD44780-based displays
 *
 * This supports all sizes of HD44780 displays, from 16x2 to 4x20. The
 * controller has no idea of the actual display hardware, so it lets you write
 * farther than you can see. These displays with such controllers are available
 * from various manufacturers with different I2C addresses. Adafruit*
 * TC1602A-01T seems to be a well-documented example. The driver also supports
 * parallel GPIO connections directly to the HD44780 in case you are not using
 * an I2C expander/backpack.
 *
 * @image html lcm1602.jpeg
 * Example for LCM1602 displays that use the I2C bus
 * @snippet lcm1602-i2c.cxx Interesting
 * Code sample for GPIO based LCM1602 displays
 * @snippet lcm1602-parallel.cxx Interesting
 */
    class Lcm1602
    {
    public:
        /**
         * Lcm1602 constructor; calls libmraa initialisation functions
         *
         * @param bus I2C bus to use. Default 0 (autodetect).
         * @param address Slave address the LCD is registered on. Default 0x27
         * @param isExpander True if we are dealing with an I2C expander,
         * false otherwise. Default is true.
         * @param numColumns Number of columns the display has. Default 16.
         * @param numRows Number of rows the display has. Default 2.
         */
        Lcm1602(int bus = 0, int address = 0x27, bool isExpander=true,
                uint8_t numColumns = 16, uint8_t numRows = 2);

        /**
         * Lcm1602 alternate constructor, used for GPIO based HD44780
         * controllers supporting RS, Enable, and 4 data pins in 4-bit
         * mode.
         *
         * @param rs Register select pin
         * @param enable Enable pin
         * @param d0 Data 0 pin
         * @param d1 Data 1 pin
         * @param d2 Data 2 pin
         * @param d3 Data 3 pin
         * @param numColumns Number of columns the display has. Default 16.
         * @param numRows Number of rows the display has. Default 2.
         */
        Lcm1602(uint8_t rs,  uint8_t enable,
                uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
                uint8_t numColumns = 16, uint8_t numRows = 2);

        /**
         * Lcm1602 destructor
         */
        ~Lcm1602();

        /**
         * Writes a string to the LCD
         *
         * @param msg std::string to write to the display; note: only ASCII
         * characters are supported
         * @return Result of the operation
         */
        upm_result_t write(std::string msg);

        /**
         * Sets the cursor to specified coordinates
         *
         * @param row Row to set the cursor to
         * @param column Column to set the cursor to
         * @return Result of the operation
         */
        upm_result_t setCursor(int row, int column);

        /**
         * Clears the display of all characters
         *
         * @return Result of the operation
         */
        upm_result_t clear();

        /**
         * Returns to the original coordinates (0,0)
         *
         * @return Result of the operation
         */
        upm_result_t home();

        /**
         * Create a custom character
         *
         * @param charSlot the character slot to write, only 8 are available
         * @param charData The character data (8 bytes) making up the character
         * @return Result of operation
         */
        upm_result_t createChar(uint8_t charSlot,
                                lcm1602_custom_char_t charData);

        /**
         * Turn the display on
         *
         * @return Result of operation
         */
        upm_result_t displayOn();

        /**
         * Turn the display off
         *
         * @return Result of operation
         */
        upm_result_t displayOff();

        /**
         * Turn the cursor on
         *
         * @return Result of operation
         */
        upm_result_t cursorOn();

        /**
         * Turn the cursor off
         *
         * @return Result of operation
         */
        upm_result_t cursorOff();

        /**
         * Turn cursor blink on
         *
         * @return Result of operation
         */
        upm_result_t cursorBlinkOn();

        /**
         * Turn cursor blink off
         *
         * @return Result of operation
         */
        upm_result_t cursorBlinkOff();

        /**
        * Turn backlight on
        *
        * @return Result of operation
        */
        upm_result_t backlightOn();

        /**
         * Turn backlight off
         *
         * @return Result of operation
         */
        upm_result_t backlightOff();

        /**
         * Scroll the display left, without changing the character RAM
         *
         * @return Result of operation
         */
        upm_result_t scrollDisplayLeft();

        /**
         * Scroll the display right, without changing the character RAM
         *
         * @return Result of operation
         */
        upm_result_t scrollDisplayRight();

        /**
         * set the entry mode so that characters are added left to right
         *
         * @return Result of operation
         */
        upm_result_t entryLeftToRight();

        /**
         * set the entry mode so that characters are added right to left
         *
         * @return Result of operation
         */
        upm_result_t entryRightToLeft();

        /**
         * Right justify text entered from the cursor
         *
         * @return Result of operation
         */
        upm_result_t autoscrollOn();

        /**
         * Left justify text entered from the cursor
         *
         * @return Result of operation
         */
        upm_result_t autoscrollOff();


    protected:
        lcm1602_context m_lcm1602;
        upm_result_t command(uint8_t cmd);
        upm_result_t data(uint8_t data);

    private:
    };
}
