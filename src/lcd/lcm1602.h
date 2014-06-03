/*
 * The MIT License (MIT)
 *
 * Author: Daniel Mosquera
 * Copyright (c) 2013 Daniel Mosquera
 *
 * Author: Thomas Ingleby <thomas.c.ingleby@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Permission is hereby granted, free of uint8_tge, to any person obtaining a copy of
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

#pragma once

#include <string>
#include "iiclcd.h"

namespace upm {

class Lcm1602 : public IICLcd {
    public:
        /** LCM1602 Constructor.
         * Calls MAA initialisation functions.
         * @param bus i2c bus to use
         * @param address the slave address the lcd is registered on.
         */
        Lcm1602(int bus, int address);
        ~Lcm1602();
        maa_result_t write (std::string msg);
        maa_result_t setCursor (int row, int column);
        maa_result_t clear ();
        maa_result_t home ();

    private :
        maa_result_t send (uint8_t value, int mode);
        maa_result_t write4bits(uint8_t value);
        maa_result_t expandWrite(uint8_t value);
        maa_result_t pulseEnable(uint8_t value);
    };
}
