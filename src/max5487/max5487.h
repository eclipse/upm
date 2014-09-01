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
#include <mraa/spi.h>

#define HIGH                    1
#define LOW                     0

namespace upm {

/**
 * @brief C++ API for MAX5487 chip (SPI digital potentionmeter)
 *
 * The Maxim Integrated
 * [MAX5487](http://datasheets.maximintegrated.com/en/ds/MAX5487-MAX5489.pdf)
 * is a Dual, 256-Tap, Nonvolatile, SPI-Interface, Linear-Taper Digital
 * Potentiometer. This module was tested on the  Maxim Integrated [MAX5487PMB1
 * PMOD module](http://datasheets.maximintegrated.com/en/ds/MAX5487PMB1.pdf)
 * from the analog PMOD kit.
 *
 * @snippet max5487.cxx Interesting
 * @image html max5487.jpeg
 */
class MAX5487 {
    public:
        static const uint8_t R_WR_WIPER_A   = 0x01;
        static const uint8_t R_WR_WIPER_B   = 0x02;

        /**
         * Instanciates a MAX5487 object
         *
         * @param csn to use if any, by default will use ICSP CS (-1)
         */
        MAX5487 (int csn);

        /**
         * MAX5487 object destructor, closes all IO connections
         */
        ~MAX5487 ();

        /**
         * Set wiper for port A.
         */
        void setWiperA (uint8_t wiper);

        /**
         * Set wiper for port B.
         */
        void setWiperB (uint8_t wiper);

        /**
         * Return name of the component
         */
        std::string name()
        {
            return m_name;
        }
    private:
        std::string m_name;
        mraa_spi_context        m_spi;
        mraa_gpio_context       m_csnPinCtx;

        /**
         * Set chip select pin LOW
         */
        mraa_result_t CSOn ();

        /**
         * Set chip select pin HIGH
         */
        mraa_result_t CSOff ();
};

}
