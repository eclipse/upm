/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string>
#include <mraa/i2c.hpp>
#include "lcd.hpp"
#include "ssd.hpp"

namespace upm
{
const uint8_t DISPLAY_CMD_SET_NORMAL_1308 = 0xA6;

/**
 * @library lcd
 * @sensor ssd1308
 * @comname OLED Display
 * @altname Grove OLED Display 0.96"
 * @type display
 * @man seeed adafruit
 * @web http://wiki.seeed.cc/Grove-OLED_Display_0.96inch/
 * @web http://www.seeedstudio.com/wiki/Grove_-_OLED_Display_0.96%22
 * @con i2c
 *
 * @brief API for SSD1308 I2C-controlled OLED displays
 *
 * SSD1308 is a 128x64 dot-matrix OLED/PLED segment driver with a
 * controller. This implementation was tested using the Grove LED 128*64
 * Display module, which is an OLED monochrome display.
 *
 * @image html ssd1308.jpeg
 * @snippet lcd-ssd1308-oled.cxx Interesting
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
