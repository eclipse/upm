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
#include "iiclcd.h"

namespace upm {

class Jhd1313m1 : public IICLcd {
    public:
        Jhd1313m1 (int bus, int lcdAddress, int rgbAddress);
        ~Jhd1313m1 ();
        maa_result_t write (std::string msg);
        maa_result_t setCursor (int row, int column);
        maa_result_t clear ();
        maa_result_t home ();

    private:
        maa_result_t cmd (maa_i2c_context ctx, uint8_t value);
        maa_result_t setReg (maa_i2c_context ctx, int deviceAdress, int addr, uint8_t data);

        int m_rgb_address;
        maa_i2c_context m_i2c_lcd_rgb;
};

}
