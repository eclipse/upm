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

I2CLcd::I2CLcd(int bus, int lcdAddress)
{
    m_lcd_control_address = lcdAddress;
    m_bus = bus;

    m_i2c_lcd_control = mraa_i2c_init(m_bus);

    mraa_result_t ret = mraa_i2c_address(m_i2c_lcd_control, m_lcd_control_address);
    if (ret != MRAA_SUCCESS) {
        fprintf(stderr, "Messed up i2c bus\n");
    }
}

mraa_result_t
I2CLcd::write(int row, int column, std::string msg)
{
    setCursor(row, column);
    write(msg);
}

mraa_result_t
I2CLcd::createChar(uint8_t charSlot, uint8_t charData[])
{
    mraa_result_t error = MRAA_SUCCESS;
    charSlot &= 0x07; // only have 8 positions we can set
    error = i2Cmd(m_i2c_lcd_control, LCD_SETCGRAMADDR | (charSlot << 3));
    if (error == MRAA_SUCCESS) {
        for (int i = 0; i < 8; i++) {
            error = i2cData(m_i2c_lcd_control, charData[i]);
        }
    }

    return error;
}

mraa_result_t
I2CLcd::close()
{
    return mraa_i2c_stop(m_i2c_lcd_control);
}

std::string
I2CLcd::name()
{
    return m_name;
}

mraa_result_t
I2CLcd::i2cReg(mraa_i2c_context ctx, int deviceAdress, int addr, uint8_t value)
{
    mraa_result_t error = MRAA_SUCCESS;

    uint8_t data[2] = { addr, value };
    error = mraa_i2c_address(ctx, deviceAdress);
    error = mraa_i2c_write(ctx, data, 2);

    return error;
}

mraa_result_t
I2CLcd::i2Cmd(mraa_i2c_context ctx, uint8_t value)
{
    mraa_result_t error = MRAA_SUCCESS;

    uint8_t data[2] = { LCD_CMD, value };
    error = mraa_i2c_address(ctx, m_lcd_control_address);
    error = mraa_i2c_write(ctx, data, 2);

    return error;
}

mraa_result_t
I2CLcd::i2cData(mraa_i2c_context ctx, uint8_t value)
{
    mraa_result_t error = MRAA_SUCCESS;

    uint8_t data[2] = { LCD_DATA, value };
    error = mraa_i2c_address(ctx, m_lcd_control_address);
    error = mraa_i2c_write(ctx, data, 2);

    return error;
}
