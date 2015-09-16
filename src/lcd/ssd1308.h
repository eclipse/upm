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
#include <mraa/i2c.hpp>
#include "lcd.h"
#include "ssd.h"

namespace upm
{
const uint8_t DISPLAY_CMD_SET_NORMAL_1308 = 0xA6;

/**
 * @library i2clcd
 * @sensor ssd1308
 * @comname SSD1308 OLED Display
 * @altname Grove OLED Display 0.96"
 * @type display
 * @man seeed adafruit
 * @web http://garden.seeedstudio.com/images/4/46/SSD1308_1.0.pdf
 * @web http://www.seeedstudio.com/wiki/Grove_-_OLED_Display_0.96%22
 * @con i2c
 *
 * @brief API for SSD1308 I2C-controlled OLED displays
 *
 * SSD1308 is a 128x64 dot-matrix OLED/PLED segment driver with a
 * controller. This implementation was tested using the Grove LED 128×64
 * Display module, which is an OLED monochrome display.
 *
 * @image html ssd1308.jpeg
 * @snippet ssd1308-oled.cxx Interesting
 */
class SSD1308 : public LCD
{
  public:
    /**
     * SSD1308 constructor; calls libmraa initialisation functions
     *
     * @param bus I2C bus to use
     * @param address Slave address the LCD is registered on
     */
    SSD1308(int bus, int address = 0x3C);
    /**
     * SSD1308 destructor
     */
    ~SSD1308();
    /**
     * Draws an image; see examples/python/make_oled_pic.py for an
     * explanation of how pixels are mapped to bytes
     *
     * @param data Buffer to read
     * @param bytes Number of bytes to read from the pointer
     * @return Result of the operation
     */
    mraa::Result draw(uint8_t* data, int bytes);
    /**
     * Writes a string to the LCD
     *
     * @param msg std::string to write to the display; note: only ASCII
     * characters are supported
     * @return Result of the operation
     */
    mraa::Result write(std::string msg);
    /**
     * Sets the cursor to specified coordinates
     *
     * @param row Row to set the cursor to
     * @param column Column to set the cursor to
     * @return Result of the operation
     */
    mraa::Result setCursor(int row, int column);
    /**
     * Clears the display of all characters
     *
     * @return Result of the operation
     */
    mraa::Result clear();
    /**
     * Returns to the original coordinates (0,0)
     *
     * @return Result of the operation
     */
    mraa::Result home();

  private:
    mraa::Result writeChar(uint8_t value);
    mraa::Result setNormalDisplay();
    mraa::Result setAddressingMode(displayAddressingMode mode);

    int m_lcd_control_address;
    mraa::I2c m_i2c_lcd_control;
};
}
