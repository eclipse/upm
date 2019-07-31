/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <string>
#include <mraa/aio.hpp>

#include <mraa/gpio.hpp>
#include <mraa/spi.hpp>

#define HIGH                    1
#define LOW                     0

namespace upm {

/**
 * @brief MAX5487 Digital Potentiometer
 * @defgroup max5487 libupm-max5487
 * @ingroup maxim spi digipot
 */
/**
 * @library max5487
 * @sensor max5487
 * @comname Digital Potentiometer
 * @type digipot
 * @man maxim
 * @con spi
 * @web https://www.maximintegrated.com/en/products/analog/data-converters/digital-potentiometers/MAX5487.html
 *
 * @brief API for the MAX5487 SPI Digital Potentiometer
 *
 * Maxim Integrated*
 * [MAX5487](http://datasheets.maximintegrated.com/en/ds/MAX5487-MAX5489.pdf)
 * is a dual, 256-tap, nonvolatile, SPI, linear-taper digital
 * potentiometer. This module was tested on the  Maxim Integrated [MAX5487PMB1
 * PMOD module](http://datasheets.maximintegrated.com/en/ds/MAX5487PMB1.pdf)
 * from the analog PMOD kit.
 *
 * @snippet max5487.cxx Interesting
 */
class MAX5487 {
    public:
        static const uint8_t R_WR_WIPER_A   = 0x01;
        static const uint8_t R_WR_WIPER_B   = 0x02;

        /**
         * Instantiates an MAX5487 object
         *
         * @param csn CSN to use, if any; by default, ICSP CS (-1) is used
         */
        MAX5487 (int csn = -1);

        /**
         * MAX5487 object destructor, closes all IO connections
         * no more needed as the connections will be closed when
         * m_spi and m_csnPinCtx will go out of scope
         * ~MAX5487 ();
         **/

        /**
         * Sets a wiper for port A.
         */
        void setWiperA (uint8_t wiper);

        /**
         * Sets a wiper for port B.
         */
        void setWiperB (uint8_t wiper);

        /**
         * Returns the name of the component
         */
        std::string name()
        {
            return m_name;
        }
    private:
        std::string m_name;
        mraa::Spi        m_spi;
        mraa::Gpio       m_csnPinCtx;

        /**
         * Sets the chip select pin to LOW
         */
        mraa::Result CSOn ();

        /**
         * Sets the chip select pin to HIGH
         */
        mraa::Result CSOff ();
};

}
