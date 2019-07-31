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
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string>
#include <vector>
#include "jhd1313m1.h"

namespace upm
{
/**
 * @brief JHD1313M1 Display Controller
 * @defgroup jhd1313m1 libupm-jhd1313m1
 * @ingroup seeed i2c display gsk
 */

/**
 * @library jhd1313m1
 * @sensor jhd1313m1
 * @comname LCD Display Driver for the JHD1313M1 Controller for HD44780-based Displays
 * @altname Grove RGB LCD
 * @type display
 * @man seeed adafruit sparkfun
 * @web http://www.seeedstudio.com/depot/Grove-LCD-RGB-Backlight-p-1643.html?cPath=34_36
 * @con i2c
 * @kit gsk
 *
 * @brief API for the JHD1313M1 I2C controller for HD44780-based displays with
 * an RGB backlight, such as a Grove RGB I2C LCD display
 *
 * JHD1313M1 has two I2C addreses: one belongs to a controller, very
 * similar to the upm::Lcm1602 LCD driver, that controls the
 * HD44780-based display, and the other controls only the
 * backlight. This module was tested with the Seeed Grove LCD RGB
 * Backlight v2.0 display that requires 5V to operate.
 *
 * @image html grovergblcd.jpg
 * @snippet jhd1313m1-lcd.cxx Interesting
 */

    class Jhd1313m1
    {
    public:
        /**
         * Jhd1313m1 constructor
         *
         * @param bus I2C bus to use
         * @param lcdAddress Slave address the LCD is registered on
         * @param rgbAddress Slave address the RGB backlight is registered on
         */
        Jhd1313m1(int bus, int lcdAddress = 0x3E, int rgbAddress = 0x62);

        /**
         * Jhd1313m1 destructor
         */
        ~Jhd1313m1();

        /**
         * Writes a string to the LCD
         *
         * @param msg std::string to write to the display; note: only ASCII
         * characters are supported
         * @return Result of the operation
         */
        upm_result_t write(std::string msg);

        /**
         * Makes the LCD scroll text
         *
         * @param direction True if scrolling to the right, false otherwise
         * @return Result of the operation
         */
        upm_result_t scroll(bool direction);

        /**
         * Sets the color of the backlight
         * Can be used to change the color even when the backlight is off
         *
         * @param r 0-255 value for red
         * @param g 0-255 value for green
         * @param b 0-255 value for blue
         * @return Result of the operation
         */
        upm_result_t setColor(uint8_t r, uint8_t g, uint8_t b);

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
         * @param charData A vector containing 8 bytes making up the character
         * @return Result of operation
         */
        upm_result_t createChar(uint8_t charSlot,
                                std::vector<uint8_t> charData);

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
        jhd1313m1_context m_jhd1313m1;
        upm_result_t command(uint8_t cmd);
        upm_result_t data(uint8_t data);

    private:
        /* Disable implicit copy and assignment operators */
        Jhd1313m1(const Jhd1313m1&) = delete;
        Jhd1313m1 &operator=(const Jhd1313m1&) = delete;
    };
}
