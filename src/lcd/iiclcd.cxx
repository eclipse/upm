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

#include "iiclcd.h"

using namespace upm;

IICLcd::IICLcd (int bus, int lcdAddress) {
	m_lcd_control_address = lcdAddress;
    m_bus = bus;

    m_i2c_lcd_control = maa_i2c_init(m_bus);

    maa_result_t ret = maa_i2c_address(m_i2c_lcd_control, m_lcd_control_address);
	if (ret != MAA_SUCCESS) {
        fprintf(stderr, "Messed up i2c bus\n");
    }
}

maa_result_t
IICLcd::write (int row, int column, std::string msg) {
	setCursor (row, column);
	write (msg);
}

maa_result_t
IICLcd::close() {
	return maa_i2c_stop(m_i2c_lcd_control);
}
