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
#include <maa/aio.h>
#include <maa/gpio.h>

//
//      A
//     ---
//  F |   | B
//     -G-
//  E |   | C
//     ---
//      D

#define SEG_A               0b00000001
#define SEG_B               0b00000010
#define SEG_C               0b00000100
#define SEG_D               0b00001000
#define SEG_E               0b00010000
#define SEG_F               0b00100000
#define SEG_G               0b01000000

#define TM1637_I2C_COMM1    0x40
#define TM1637_I2C_COMM2    0xC0
#define TM1637_I2C_COMM3    0x80

#define PULSE_LENGTH        50

#define HIGH                  1
#define LOW                    0

namespace upm {

class TM1637 {
    public:
        TM1637 (uint8_t di, uint8_t dcki);
        ~TM1637 ();
        maa_result_t setBrightness (uint8_t level);
        maa_result_t setSegments (const uint8_t segments[], uint8_t length = 4, uint8_t pos = 0);
        maa_result_t write (std::string msg);
        
        std::string name()
        {
            return m_name;
        }
    private:
        maa_result_t start();
        maa_result_t stop();
        maa_result_t writeByte (uint8_t value);
        maa_result_t pinMode (maa_gpio_context ctx, gpio_dir_t mode);

        std::string m_name;
        maa_gpio_context m_clkPinCtx;
        maa_gpio_context m_dataPinCtx;
        uint8_t m_brightness;
};

}
