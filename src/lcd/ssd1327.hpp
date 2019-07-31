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
const uint8_t DISPLAY_CMD_SET_NORMAL = 0xA4;

/**
 * @library lcd
 * @sensor ssd1327
 * @comname OLED Display
 * @altname Grove OLED Display 1.12"
 * @type display
 * @man seeed adafruit
 * @web http://wiki.seeed.cc/Grove-OLED_Display_1.12inch/
 * @con i2c
 *
 * @brief API for SSD1327 I2C-controlled OLED displays
 *
 * SSD1327 is a 96x96 dot-matrix OLED/PLED segment driver with a controller.
 * This implementation was tested using the Grove LED 96*96 Display module,
 * which is an OLED monochrome display.
 *
 * @image html ssd1327.jpeg
 * @snippet lcd-ssd1327-oled.cxx Interesting
 */
class SSD1327 : public LCD
{
  public:
    /**
     * SSD1327 constructor; calls libmraa initialisation functions
     *
     * @param bus I2C bus to use
     * @param address Slave address the LCD is registered on
     */
    SSD1327(int bus, int address = 0x3C);
    /**
     * SSD1327 destructor
     */
    ~SSD1327();
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
     * Sets the gray level for the LCD panel
     *
     * @param level level from 0 to 255
     * @return Result of the operation
     */
    void setGrayLevel(uint8_t level);
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
    mraa::Result setHorizontalMode();
    mraa::Result setVerticalMode();

    uint8_t grayHigh;
    uint8_t grayLow;

    int m_lcd_control_address;
    mraa::I2c m_i2c_lcd_control;
};
}
