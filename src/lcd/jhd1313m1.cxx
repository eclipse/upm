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

#include <iostream>
#include <stdexcept>
#include <unistd.h>

#include "lcd_private.h"
#include "jhd1313m1.h"

using namespace upm;

Jhd1313m1::Jhd1313m1(int bus, int lcdAddress, int rgbAddress)
        : LCD(bus, lcdAddress), m_i2c_lcd_rgb(bus)
{
    m_rgb_address = rgbAddress;

    mraa_result_t ret = m_i2c_lcd_rgb.address(m_rgb_address);
    if (ret != MRAA_SUCCESS) {
        fprintf(stderr, "Messed up i2c bus\n");
    }

    usleep(50000);
    ret = m_i2c_lcd_control.writeReg(LCD_CMD, LCD_FUNCTIONSET | LCD_2LINE);

    if (!ret) {
        ret = m_i2c_lcd_control.writeReg(LCD_CMD, LCD_FUNCTIONSET | LCD_2LINE);
        UPM_CHECK_MRAA_SUCCESS(ret, "Unable to initialise the LCD controller");
    }

    usleep(100);
    ret = m_i2c_lcd_control.writeReg(LCD_CMD, LCD_DISPLAYCONTROL | LCD_DISPLAYON);
    UPM_CHECK_MRAA_SUCCESS(ret, "Unable to initialise the LCD controller");

    usleep(100);
    ret = clear();
    UPM_CHECK_MRAA_SUCCESS(ret, "Unable to initialise the LCD controller");

    usleep(2000);
    ret =
    m_i2c_lcd_control.writeReg(LCD_CMD, LCD_ENTRYMODESET | LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT);
    UPM_CHECK_MRAA_SUCCESS(ret, "Unable to initialise the LCD controller");

    ret = m_i2c_lcd_rgb.writeReg(0, 0);
    UPM_CHECK_MRAA_SUCCESS(ret, "Unable to initialise the RGB controller");
    ret = m_i2c_lcd_rgb.writeReg(1, 0);
    UPM_CHECK_MRAA_SUCCESS(ret, "Unable to initialise the RGB controller");
    ret = m_i2c_lcd_rgb.writeReg(0x08, 0xAA);
    UPM_CHECK_MRAA_SUCCESS(ret, "Unable to initialise the RGB controller");

    ret = m_i2c_lcd_rgb.writeReg(0x04, 0xFF);
    UPM_CHECK_MRAA_SUCCESS(ret, "Unable to initialise the RGB controller");
    ret = m_i2c_lcd_rgb.writeReg(0x03, 0xFF);
    UPM_CHECK_MRAA_SUCCESS(ret, "Unable to initialise the RGB controller");
    ret = m_i2c_lcd_rgb.writeReg(0x02, 0xFF);
    UPM_CHECK_MRAA_SUCCESS(ret, "Unable to initialise the RGB controller");
}

Jhd1313m1::~Jhd1313m1()
{
    clear();
    setColor(0x00, 0x00, 0x00);
}

mraa_result_t
Jhd1313m1::setColor(uint8_t r, uint8_t g, uint8_t b)
{
    mraa_result_t ret;

    ret = m_i2c_lcd_rgb.writeReg(0, 0);
    UPM_GOTO_ON_MRAA_FAIL(ret, beach);
    ret = m_i2c_lcd_rgb.writeReg(1, 0);
    UPM_GOTO_ON_MRAA_FAIL(ret, beach);
    ret = m_i2c_lcd_rgb.writeReg(0x08, 0xAA);
    UPM_GOTO_ON_MRAA_FAIL(ret, beach);

    ret = m_i2c_lcd_rgb.writeReg(0x04, r);
    UPM_GOTO_ON_MRAA_FAIL(ret, beach);
    ret = m_i2c_lcd_rgb.writeReg(0x03, g);
    UPM_GOTO_ON_MRAA_FAIL(ret, beach);
    ret = m_i2c_lcd_rgb.writeReg(0x02, b);

beach:
    return ret;
}

mraa_result_t
Jhd1313m1::scroll(bool direction)
{
    if (direction) {
        return m_i2c_lcd_control.writeReg(LCD_CMD, LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
    } else {
        return m_i2c_lcd_control.writeReg(LCD_CMD,
                                          LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
    }
}

/*
 * **************
 *  virtual area
 * **************
 */
mraa_result_t
Jhd1313m1::write(std::string msg)
{
    mraa_result_t ret = MRAA_SUCCESS;

    // This usleep fixes an odd bug where the clear function doesn't always work properly
    usleep(1000);

    for (std::string::size_type i = 0; i < msg.size(); ++i) {
        ret = m_i2c_lcd_control.writeReg(LCD_DATA, msg[i]);
        UPM_GOTO_ON_MRAA_FAIL(ret, beach);
    }

beach:
    return ret;
}

mraa_result_t
Jhd1313m1::setCursor(int row, int column)
{
    mraa_result_t ret;

    int row_addr[] = { 0x80, 0xc0, 0x14, 0x54 };
    uint8_t offset = ((column % 16) + row_addr[row]);

    ret = m_i2c_lcd_control.writeReg(LCD_CMD, offset);

    return ret;
}

mraa_result_t
Jhd1313m1::clear()
{
    return m_i2c_lcd_control.writeReg(LCD_CMD, LCD_CLEARDISPLAY);
}

mraa_result_t
Jhd1313m1::home()
{
    return m_i2c_lcd_control.writeReg(LCD_CMD, LCD_RETURNHOME);
}
