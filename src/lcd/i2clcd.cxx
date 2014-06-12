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

#include "i2clcd.h"

using namespace upm;

I2CLcd::I2CLcd (int bus, int lcdAddress) {
    m_lcd_control_address = lcdAddress;
    m_bus = bus;

    m_i2c_lcd_control = maa_i2c_init(m_bus);

    maa_result_t ret = maa_i2c_address(m_i2c_lcd_control, m_lcd_control_address);
    if (ret != MAA_SUCCESS) {
        fprintf(stderr, "Messed up i2c bus\n");
    }
}

maa_result_t
I2CLcd::write (int row, int column, std::string msg) {
    setCursor (row, column);
    write (msg);
}

maa_result_t
I2CLcd::close() {
    return maa_i2c_stop(m_i2c_lcd_control);
}

maa_result_t
I2CLcd::i2cReg (maa_i2c_context ctx, int deviceAdress, int addr, uint8_t value) {
    maa_result_t error = MAA_SUCCESS;

    uint8_t data[2] = { addr, value };
    error = maa_i2c_address (ctx, deviceAdress);
    error = maa_i2c_write (ctx, data, 2);

    return error;
}

maa_result_t
I2CLcd::i2Cmd (maa_i2c_context ctx, uint8_t value) {
    maa_result_t error = MAA_SUCCESS;

    uint8_t data[2] = { LCD_CMD, value };
    error = maa_i2c_address (ctx, m_lcd_control_address);
    error = maa_i2c_write (ctx, data, 2);

    return error;
}

maa_result_t
I2CLcd::i2cData (maa_i2c_context ctx, uint8_t value) {
    maa_result_t error = MAA_SUCCESS;

    uint8_t data[2] = { LCD_DATA, value };
    error = maa_i2c_address (ctx, m_lcd_control_address);
    error = maa_i2c_write (ctx, data, 2);

    return error;
}
