/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Contributions: Jon Trulson <jtrulson@ics.com>
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
 * @library i2clcd
 * @sensor jhd1313m1
 * @comname JHD1313M1 Display
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
 * JHD1313M1 has two I2C addreses: one belongs to a controller, very similar
 * to the upm::Lcm1602 LCD driver, that controls the HD44780-based display, and the
 * other controls only the backlight. This module was tested with the Seeed
 * Grove LCD RGB Backlight v2.0 display that requires 5V to operate.
 *
 * @image html grovergblcd.jpg
 * @snippet jhd1313m1-lcd.cxx Interesting
 */
class Jhd1313m1 : public Lcm1602
{
  public:
    /**
     * Jhd1313m1 constructor
     *
     * @param bus I2C bus to use
     * @param address Slave address the LCD is registered on
     * @param address Slave address the RGB backlight is registered on
     */
    Jhd1313m1(int bus, int lcdAddress = 0x3E, int rgbAddress = 0x62);
    /**
     * Jhd1313m1 destructor
     */
    ~Jhd1313m1();
    /**
     * Makes the LCD scroll text
     *
     * @param direction True if scrolling to the right
     * @return Result of the operation
     */
    mraa::Result scroll(bool direction);
    /**
     * Sets the color of the backlight
     *
     * @param r 0-255 value for red
     * @param g 0-255 value for green
     * @param b 0-255 value for blue
     * @return Result of the operation
     */
    mraa::Result setColor(uint8_t r, uint8_t g, uint8_t b);

 protected:
    virtual mraa::Result command(uint8_t cmd);
    virtual mraa::Result data(uint8_t data);

  private:
    int m_rgb_address;
    mraa::I2c m_i2c_lcd_rgb;
};
}
