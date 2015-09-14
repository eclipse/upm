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
#include <mraa/aio.hpp>
#include <mraa/common.hpp>

#include <mraa/gpio.hpp>

#define MAX_BIT_PER_BLOCK     16
#define CMDMODE               0x0000
#define BIT_HIGH              0x00ff
#define BIT_LOW               0x0000

#define HIGH                  1
#define LOW                   0

namespace upm {

/**
 * @brief MY9221 LED Bar library
 * @defgroup my9221 libupm-my9221
 * @ingroup seeed display gpio eak
 */
/**
 * @library my9221
 * @sensor my9221
 * @comname Grove LED Bar
 * @altname MY9221 LED Bar
 * @type display
 * @man seeed
 * @web http://www.seeedstudio.com/wiki/Grove_-_LED_Bar
 * @con gpio
 * @kit eak
 *
 * @brief API for MY9221-based LED Bars
 *
 * This module defines the MY9221 interface for libmy9221
 *
 * @image html my9221.jpg
 * @snippet my9221-ledbar.cxx Interesting
 * @snippet my9221-updown.cxx Interesting
 */
class MY9221 {
    public:
         /**
         * Instantiates an MY9221 object
         *
         * @param di Data pin
         * @param dcki Clock pin
         */
        MY9221 (uint8_t di, uint8_t dcki);

        /**
         * Sets the bar level 
         *
         * @param level Selected level for the bar (1 - 10)
         * @param direction Up or down; up is true and default
         */
        mraa::Result setBarLevel (uint8_t level, bool direction=true);

        /**
         * Returns the name of the component
         */
        std::string name()
        {
            return m_name;
        }
    private:
        mraa::Result lockData ();
        mraa::Result send16bitBlock (short data);

        std::string m_name;
        mraa::Gpio m_clkPinCtx;
        mraa::Gpio m_dataPinCtx;
};

}
