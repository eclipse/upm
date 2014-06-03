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
#include <unistd.h>

#include "jhd1313m1.h"

using namespace upm;

Jhd1313m1::Jhd1313m1 (int bus, int lcdAddress, int rgbAddress) : IICLcd(bus, lcdAddress) {
    maa_result_t error = MAA_SUCCESS;

    m_rgb_address = rgbAddress;
    m_i2c_lcd_rgb = maa_i2c_init(m_bus);

    maa_result_t ret = maa_i2c_address(m_i2c_lcd_rgb, m_rgb_address);
    if (ret != MAA_SUCCESS) {
        fprintf(stderr, "Messed up i2c bus\n");
    }

    usleep(50000);
    cmd (m_i2c_lcd_control, LCD_FUNCTIONSET | LCD_2LINE);
    usleep(4500);
    cmd (m_i2c_lcd_control, LCD_FUNCTIONSET | LCD_2LINE);
    usleep(4500);
    cmd (m_i2c_lcd_control, LCD_FUNCTIONSET | LCD_2LINE);
    usleep(4500);
    cmd (m_i2c_lcd_control, LCD_FUNCTIONSET | LCD_2LINE);

    cmd (m_i2c_lcd_control, LCD_DISPLAYCONTROL | LCD_DISPLAYON);
    clear ();
    usleep(4500);
    
    cmd (m_i2c_lcd_control,     LCD_ENTRYMODESET |
                            LCD_ENTRYLEFT |
                            LCD_ENTRYSHIFTDECREMENT);

    setReg (m_i2c_lcd_rgb, m_rgb_address, 0, 0);
    setReg (m_i2c_lcd_rgb, m_rgb_address, 1, 0);
    setReg (m_i2c_lcd_rgb, m_rgb_address, 0x08, 0xAA);

    setReg (m_i2c_lcd_rgb, m_rgb_address, 0x04, 255);
    setReg (m_i2c_lcd_rgb, m_rgb_address, 0x03, 255);
    setReg (m_i2c_lcd_rgb, m_rgb_address, 0x02, 255);
}

Jhd1313m1::~Jhd1313m1() {
    
}

/*
 * **************
 *  virtual area
 * **************
 */
maa_result_t
Jhd1313m1::write (std::string msg) {
    maa_result_t error = MAA_SUCCESS;
    uint8_t data[2] = {0x40, 0};
    for (std::string::size_type i = 0; i < msg.size(); ++i) {
        data[1] = msg[i];
        error = maa_i2c_address (m_i2c_lcd_control, m_lcd_control_address);
        error = maa_i2c_write (m_i2c_lcd_control, data, 2);
    }

    return error;
}

maa_result_t
Jhd1313m1::setCursor (int row, int column) {
    maa_result_t error = MAA_SUCCESS;

    int row_addr[] = { 0x80, 0xc0, 0x14, 0x54};
    uint8_t offset = ((column % 16) + row_addr[row]);

    uint8_t data[2] = { 0x80, offset };
    error = maa_i2c_address (m_i2c_lcd_control, m_lcd_control_address);
    error = maa_i2c_write (m_i2c_lcd_control, data, 2);

    return error;
}

maa_result_t
Jhd1313m1::clear () {
    return cmd (m_i2c_lcd_control, LCD_CLEARDISPLAY);
}

maa_result_t
Jhd1313m1::home () {
    return cmd (m_i2c_lcd_control, LCD_RETURNHOME);
}

/*
 * **************
 *  private area
 * **************
 */
maa_result_t
Jhd1313m1::setReg (maa_i2c_context ctx, int deviceAdress, int addr, uint8_t value) {
    maa_result_t error = MAA_SUCCESS;

    uint8_t data[2] = { addr, value };
    error = maa_i2c_address (ctx, deviceAdress);
    error = maa_i2c_write (ctx, data, 2);

    return error;
}

maa_result_t
Jhd1313m1::cmd (maa_i2c_context ctx, uint8_t value) {
    maa_result_t error = MAA_SUCCESS;

    uint8_t data[2] = { 0x80, value };
    error = maa_i2c_address (ctx, m_lcd_control_address);
    error = maa_i2c_write (ctx, data, 2);

    return error;
}
