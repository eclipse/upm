/*
 * The MIT License (MIT)
 *
 * Author: Daniel Mosquera
 * Copyright (c) 2013 Daniel Mosquera
 *
 * Author: Thomas Ingleby <thomas.c.ingleby@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
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

#include <string>
#include <unistd.h>

#include "lcm1602.h"

#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

#define LCD_EN 0x04 // Enable bit
#define LCD_RW 0x02 // Read/Write bit
#define LCD_RS 0x01 // Register select bit

using namespace upm;

Lcm1602::Lcm1602(int bus_in, int addr_in)
{
    address = addr_in;
    bus = bus_in;
    maa_init();
    m_i2c = maa_i2c_init(bus);

    maa_i2c_address(m_i2c, address);

    usleep(50000);
    expandWrite(LCD_BACKLIGHT);
    usleep(100000);

    write4bits(0x03 << 4);
    usleep(4500);
    write4bits(0x30);
    usleep(4500);
    write4bits(0x30);
    usleep(150);

    // Put into 4 bit mode
    write4bits(0x20);

    // Set numeber of lines
    send(LCD_FUNCTIONSET | 0x0f, 0);
    send(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF, 0);
    clear();

    // Set entry mode.
    send(LCD_ENTRYMODESET | LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT, 0);

    home();

}

int
Lcm1602::clear()
{
    return send(LCD_CLEARDISPLAY, 0);
}

int
Lcm1602::home()
{
    return send(LCD_RETURNHOME, 0);
}

int
Lcm1602::cursor(int row, int column)
{
    if (row > 3)
        return 99;
    int row_addr[] = { 0x80, 0xc0, 0x14, 0x54};
    return send(LCD_SETDDRAMADDR | ((column % 16) + row_addr[row]),0);
}

int
Lcm1602::write(std::string msg)
{
    int ret = 0;
    for(std::string::size_type i = 0; i < msg.size(); ++i) {
        ret = send(msg[i], LCD_RS);
    }
    return 0;
}

int
Lcm1602::close()
{
    return maa_i2c_stop(m_i2c);
}

int
Lcm1602::send(char value, int mode)
{
    int ret = 0;
    char h = value & 0xf0;
    char l = (value << 4) & 0xf0;
    ret = write4bits(h | mode);
    ret = write4bits(l | mode);
    return ret;
}

int
Lcm1602::write4bits(char value)
{
    int ret = 0;
    ret = expandWrite(value);
    ret = pulseEnable(value);
    return 0;
}

int
Lcm1602::expandWrite(char value)
{
    char buffer = value | LCD_BACKLIGHT;
    return maa_i2c_write_byte(m_i2c, buffer);
}

int
Lcm1602::pulseEnable(char value)
{
    int ret = 0;
    ret = expandWrite(value | LCD_EN);
    usleep(1);
    ret = expandWrite(value & ~LCD_EN);
    usleep(50);
    return ret;
}
