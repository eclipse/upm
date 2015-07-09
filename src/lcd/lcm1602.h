/*
 * The MIT License (MIT)
 *
 * Author: Daniel Mosquera
 * Copyright (c) 2013 Daniel Mosquera
 *
 * Author: Thomas Ingleby <thomas.c.ingleby@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Permission is hereby granted, free of uint8_tge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include <string>
#include "lcd.h"

namespace upm
{
/**
 * @library i2clcd
 * @sensor lcm1602
 * @comname LCM1602 Display
 * @type display
 * @man adafruit sparkfun
 * @web https://www.adafruit.com/datasheets/TC1602A-01T.pdf
 * @con i2c
 *
 * @brief API for LCM1602 i2c controller for HD44780 based displays
 *
 * This supports all sizes of HD44780 displays from 16x2 to 4x20, the
 * controller has no idea of the actual display hardware so will let you write
 * further than you can see. These displays with such controllers are available
 * from various manufacturers with different i2c addresses. The adafruit
 * TC1602A-01T seems to be a well documented example.
 *
 * @image html lcm1602.jpeg
 * @snippet lcm1602-lcd.cxx Interesting
 */
class Lcm1602 : public I2CLcd
{
  public:
    /**
     * Lcm1602 Constructor, calls libmraa initialisation functions
     *
     * @param bus i2c bus to use
     * @param address the slave address the lcd is registered on
     */
    Lcm1602(int bus, int address);
    /**
     * Lcm1602 Destructor
     */
    ~Lcm1602();
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
    mraa_result_t send(uint8_t value, int mode);
    mraa_result_t write4bits(uint8_t value);
    mraa_result_t expandWrite(uint8_t value);
    mraa_result_t pulseEnable(uint8_t value);
};
}
