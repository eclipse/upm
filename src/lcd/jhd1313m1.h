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
#include "i2clcd.h"

namespace upm {

/**
 * @brief C++ API for Jhd1313m1 i2c controller for HD44780 based displays with
 * an RGB backlight such as the Grove RGB i2c LCD display
 *
 * The Jhd1313m1 has two i2c addreses, one belongs to a controller very similar
 * to the upm::Lcm1602 LCD driver which controls the HD44780 based display and the
 * other controls solely the backlight. This module was tested with the Seed
 * [Grove LCD RGB Backlight v2.0 display]
 * (http://www.seeedstudio.com/depot/Grove-LCD-RGB-Backlight-p-1643.html?cPath=34_36),
 * which requires 5V to operate.
 *
 * @ingroup i2clcd i2c
 * @snippet jhd1313m1-lcd.cxx Interesting
 * @image html grovergblcd.jpeg
 */
class Jhd1313m1 : public I2CLcd {
    public:
       /**
        * Jhd1313m1 constructor
        *
        * @param bus i2c bus to use
        * @param address the slave address the lcd is registered on
        * @param address the slave address the rgb backlight is on
        */
        Jhd1313m1(int bus, int lcdAddress=0x3E, int rgbAddress=0x62);
       /**
        * Jhd1313m1 destructor
        */
        ~Jhd1313m1();
       /**
        * Make the LCD scroll text
        *
        * @param direction, true is typical scrolling to the right
        * @return Result of operation
        */
        mraa_result_t scroll (bool direction);
       /**
        * Set the color of the backlight
        *
        * @param r 0-255 value for red
        * @param g 0-255 value for green
        * @param b 0-255 value for blue
        * @return Result of operation
        */
        mraa_result_t setColor(uint8_t r, uint8_t g, uint8_t b);
       /**
        * Write a string to LCD
        *
        * @param msg The std::string to write to display, note only ascii
        *     chars are supported
        * @return Result of operation
        */
        mraa_result_t write(std::string msg);
       /**
        * Set cursor to a coordinate
        *
        * @param row The row to set cursor to
        * @param column The column to set cursor to
        * @return Result of operation
        */
        mraa_result_t setCursor(int row, int column);
       /**
        * Clear display from characters
        *
        * @return Result of operatio
        */
        mraa_result_t clear();
       /**
        * Return to coordinate 0,0
        *
        * @return Result of operation
        */
        mraa_result_t home();

    private:
        int m_rgb_address;
        mraa_i2c_context m_i2c_lcd_rgb;
};

}
