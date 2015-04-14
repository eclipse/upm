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

#define MAX_BIT_PER_BLOCK     16
#define CMDMODE               0x0000
#define BIT_HIGH              0x00ff
#define BIT_LOW               0x0000

#define HIGH                  1
#define LOW                   0

namespace upm {

/**
 * @brief my9221 led bar library
 * @defgroup my9221 libupm-9221
 */

/**
 * @brief C++ API for MY9221 led bar module
 *
 * This file defines the MY9221 C++ interface for libmy9221
 *
 * @ingroup my9221 gpio
 * @snippet my9221-ledbar.cxx Interesting
 * @snippet my9221-updown.cxx Interesting
 */
class MY9221 {
    public:
         /**
         * Instanciates a MY9221 object
         *
         * @param di data pin
         * @param dcki clock pin
         */
        MY9221 (uint8_t di, uint8_t dcki);

        /**
         * MY9221 object destructor
         */
        ~MY9221 ();

        /**
         * Set the level bar
         *
         * @param level selected level for the bar (1 - 10)
         * @param direction up or down, true is up and is the default
         */
        mraa_result_t setBarLevel (uint8_t level, bool direction=true);

        /**
         * Return name of the component
         */
        std::string name()
        {
            return m_name;
        }
    private:
        mraa_result_t lockData ();
        mraa_result_t send16bitBlock (short data);

        std::string m_name;
        mraa_gpio_context m_clkPinCtx;
        mraa_gpio_context m_dataPinCtx;
};

}
