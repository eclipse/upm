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
 * The MIT License
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
#include "lcdks.h"

namespace upm
{
    /**
     * @brief LCD Keypad Shield
     * @defgroup lcdks libupm-lcdks
     * @ingroup dfrobot sainsmart display
     */

    /**
     * @library lcdks
     * @sensor lcdks
     * @comname LCD Keypad Shield
     * @type display
     * @man sainsmart dfrobot sparkfun
     * @web http://www.sainsmart.com/sainsmart-1602-lcd-keypad-shield-for-arduino-duemilanove-uno-mega2560-mega1280.html
     * @web http://www.dfrobot.com/index.php?route=product/product&product_id=51
     * @web https://www.sparkfun.com/products/13293
     * @con gpio analog
     *
     * @brief API for the generic LCD Keypad Shield
     *
     * The LCD Keypad Shield uses 7 digital outputs and 1 analog input
     * (for the keypad). The outputs are used to drive an attached
     * LCM1602 LCD controller. This driver should be compatible with
     * the similar LCD keypad shields from Sainsmart, DFRobot and
     * Sparkfun.
     *
     * @image html keypadlcd.jpg
     * @snippet lcdks.cxx Interesting
     */

    class LCDKS
    {
    public:
        /**
         * LCDKS constructor
         *
         * As this is a shield, you will not likely have any choice over
         * the pins that are used.  For this reason, we provide defaults
         * for all of them -- of course they can be changed if your device
         * is different.
         *
         * @param rs Register select pin.
         * @param enable Enable pin.
         * @param d0 Data 0 pin.
         * @param d1 Data 1 pin.
         * @param d2 Data 2 pin.
         * @param d3 Data 3 pin.
         * @param keypad Analog pin of the keypad.
         * @param backlight Optional GPIO backlight pin.  Specify -1 if
         * not in use or not supported on your device.
         * @throws std::runtime_error on initialization error.
         */
        LCDKS(int rs=8, int enable=9,
              int d0=4, int d1=5, int d2=6, int d3=7,
              int keypad=0, int backlight=-1);
        /**
         * LCDKS destructor
         */
        ~LCDKS();

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

        /**
         * Returns the floating point representation of the key that is
         * being pushed.  Each key produces a different value between 0.0
         * and 1.0, and only one key can be read at a time.
         *
         * @return The floating point value representing a key.
         */
        float getKeyValue();

        /**
         * Returns the floating point representation of the key that is
         * being pushed.  Each key produces a different value between 0.0
         * and 1.0, and only one key can be read at a time.
         *
         * @deprecated This function is deprecated. Use getKeyValue() instead.
         * @return The floating point value representing a key.
         */
        float getRawKeyValue()
        {
            return getKeyValue();
        }


    protected:
        lcdks_context m_lcdks;

    private:
        /* Disable implicit copy and assignment operators */
        LCDKS(const LCDKS&) = delete;
        LCDKS &operator=(const LCDKS&) = delete;

    };
}
