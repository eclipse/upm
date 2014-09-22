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
#include <mraa/aio.h>
#include <mraa/gpio.h>

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

#define HIGH                1
#define LOW                 0

namespace upm {

/**
 * @brief tm1637 7-segment screen library
 * @defgroup tm1637 libupm-tm1637
 */

/**
 * @brief C++ API for Seven segments screen
 *
 * This file defines the TM1637 C++ interface for lib4digitdisplay
 *
 * @ingroup tm1637
 * @snippet 4digitdisplay.cxx Interesting
 *
 *      A
 *     ---
 *  F |   | B
 *     -G-
 *  E |   | C
 *     ---
 *      D
 *
 */
class TM1637 {
    public:
        /**
         * Instanciates a TM1637 object
         *
         * @param di data pin
         * @param dcki clock pin
         */
        TM1637 (uint8_t di, uint8_t dcki);
        /**
         * TM1637 object destructor, this will close all used Gpio
         * pins  (di and dcki)
         */
        ~TM1637 ();

        /**
         * Set the brightness of the seven segment display
         *
         * @param level The brightness level of leds
         */
        mraa_result_t setBrightness (uint8_t level);

        /**
         * Set the the segment screen data and number of segments
         *
         * @param segments data to write on the segments, each elemnt
         * in array is segment
         * @param length number of elements in segments array
         * @param pos data writing offset
         */
        mraa_result_t setSegments (const uint8_t segments[], uint8_t length = 4, uint8_t pos = 0);

        /**
         * Write message on the screen.
         *
         * @param msg The message to be written on the sreen
         */
        mraa_result_t write (std::string msg);

        /**
         * Return name of the component
         */
        std::string name()
        {
            return m_name;
        }

    private:
        mraa_result_t start();
        mraa_result_t stop();
        mraa_result_t writeByte (uint8_t value);
        mraa_result_t pinMode (mraa_gpio_context ctx, gpio_dir_t mode);

        mraa_gpio_context m_clkPinCtx;
        mraa_gpio_context m_dataPinCtx;

        std::string m_name;
        uint8_t m_brightness;
};

}
