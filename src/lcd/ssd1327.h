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
#include "ssd.h"

namespace upm {

#define DISPLAY_CMD_SET_NORMAL      0xA4

/**
 * @brief C++ API for SSD1327 i2c controlled OLED displays
 *
 * The [SSD1327](http://garden.seeedstudio.com/images/8/82/SSD1327_datasheet.pdf)
 * is a 96x96 Dot matrix OLED/PLED segment driver with controller. This
 * implementation was tested using the
 * [Grove LED 96×96 Display module]
 * (http://www.seeedstudio.com/wiki/Grove_-_OLED_Display_1.12%22)
 * which is an OLED monochrome display
 *
 * @ingroup i2clcd
 * @snippet oled-1327.cxx Interesting
 * @image html ssd1327.jpeg
 */
class SSD1327 : public I2CLcd {
    public:
        /**
         * SSD1327 Constructor, calls libmraa initialisation functions
         *
         * @param bus i2c bus to use
         * @param address the slave address the lcd is registered on
         */
        SSD1327 (int bus, int address=0x3C);
       /**
        * SSD1327 destructor
        */
        ~SSD1327 ();
       /**
        * Draw an image, see examples/python/make_oled_pic.py for an
        * explanation on how the pixels are mapped to bytes
        *
        * @param data the buffer to read
        * @param bytes the amount of bytes to read from the pointer
        * @return Result of operation
        */
        mraa_result_t draw(uint8_t *data, int bytes);
       /**
        * Set gray level for LCD panel
        *
        * @param gray level from 0-255
        * @return Result of operation
        */
        mraa_result_t setGrayLevel (uint8_t level);
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
        mraa_result_t writeChar (mraa_i2c_context ctx, uint8_t value);
        mraa_result_t setNormalDisplay ();
        mraa_result_t setHorizontalMode ();
        mraa_result_t setVerticalMode ();

        uint8_t grayHigh;
        uint8_t grayLow;
    };
}
