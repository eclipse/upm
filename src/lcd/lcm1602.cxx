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

using namespace upm;

Lcm1602::Lcm1602(int bus_in, int addr_in) : IICLcd (bus_in, addr_in) {
    maa_result_t error = MAA_SUCCESS;

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

Lcm1602::~Lcm1602 () {
    
}

/*
 * **************
 *  virtual area
 * **************
 */
maa_result_t
Lcm1602::write (std::string msg) {
    maa_result_t error = MAA_SUCCESS;
    for (std::string::size_type i = 0; i < msg.size(); ++i) {
        error = send (msg[i], LCD_RS);
    }
    return error;
}

maa_result_t
Lcm1602::setCursor (int row, int column) {
    maa_result_t error = MAA_SUCCESS;

    int row_addr[] = { 0x80, 0xc0, 0x14, 0x54};
    uint8_t offset = ((column % 16) + row_addr[row]);

    return send (LCD_CMD | offset, 0);
}

maa_result_t
Lcm1602::clear () {
    return send(LCD_CLEARDISPLAY, 0);
}

maa_result_t
Lcm1602::home () {
    return send(LCD_RETURNHOME, 0);
}

/*
 * **************
 *  private area
 * **************
 */
maa_result_t
Lcm1602::send (uint8_t value, int mode) {
    maa_result_t ret = MAA_SUCCESS;
    uint8_t h = value & 0xf0;
    uint8_t l = (value << 4) & 0xf0;
    ret = write4bits(h | mode);
    ret = write4bits(l | mode);
    return ret;
}

maa_result_t
Lcm1602::write4bits(uint8_t value)
{
    maa_result_t ret = MAA_SUCCESS;
    ret = expandWrite(value);
    ret = pulseEnable(value);
    return ret;
}

maa_result_t
Lcm1602::expandWrite(uint8_t value)
{
    uint8_t buffer = value | LCD_BACKLIGHT;
    return maa_i2c_write_byte(m_i2c_lcd_control, buffer);
}

maa_result_t
Lcm1602::pulseEnable(uint8_t value)
{
    maa_result_t ret = MAA_SUCCESS;
    ret = expandWrite(value | LCD_EN);
    usleep(1);
    ret = expandWrite(value & ~LCD_EN);
    usleep(50);
    return ret;
}
