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
#include <mraa/i2c.h>

namespace upm
{
// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_DATA 0x40
#define LCD_CMD 0x80

#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for CGRAM
#define LCD_SETCGRAMADDR 0x40

#define LCD_EN 0x04 // Enable bit
#define LCD_RW 0x02 // Read/Write bit
#define LCD_RS 0x01 // Register select bit

/**
 * @brief i2c LCD display library
 * @defgroup i2clcd libupm-i2clcd
 * @ingroup seeed sparkfun adafruit i2c display gsk
 */
class I2CLcd
{
  public:
    I2CLcd(int bus, int lcdAddress);
    mraa_result_t write(int x, int y, std::string msg);

    virtual mraa_result_t write(std::string msg) = 0;
    virtual mraa_result_t setCursor(int row, int column) = 0;
    virtual mraa_result_t clear() = 0;
    virtual mraa_result_t home() = 0;
    virtual mraa_result_t createChar(uint8_t charSlot, uint8_t charData[]);
    virtual mraa_result_t i2Cmd(mraa_i2c_context ctx, uint8_t value);
    virtual mraa_result_t i2cReg(mraa_i2c_context ctx, int deviceAdress, int addr, uint8_t data);
    virtual mraa_result_t i2cData(mraa_i2c_context ctx, uint8_t value);

    mraa_result_t close();
    std::string name();

  protected:
    std::string m_name;
    int m_lcd_control_address;
    int m_bus;
    mraa_i2c_context m_i2c_lcd_control;
};
}
