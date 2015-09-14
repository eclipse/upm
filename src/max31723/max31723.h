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

#include <mraa/gpio.hpp>

#include <mraa/spi.hpp>

#define HIGH                    1
#define LOW                     0

namespace upm {

/**
 * @brief MAX31723 Temperature Sensor library
 * @defgroup max31723 libupm-max31723
 * @ingroup maxim spi temperature
 */
/**
 * @library max31723
 * @sensor max31723
 * @comname MAX31723 Temperature Sensor
 * @type temperature
 * @man maxim
 * @con spi
 *
 * @brief API for the MAX31723 Temperature Sensor
 *
 * Maxim Integrated*
 * [MAX31723](http://datasheets.maximintegrated.com/en/ds/MAX31722-MAX31723.pdf)
 * is a low-voltage 3-wire/SPI temperature sensor controller. This module was
 * tested on the Maxim Integrated [MAX31732PMB1 PMOD
 * module](http://datasheets.maximintegrated.com/en/ds/MAX31723PMB1.pdf) from
 * the analog PMOD kit.
 *
 * @snippet max31723.cxx Interesting
 */
class MAX31723 {
    public:
        static const uint8_t R_STS_READ_CMD     = 0x00;
        static const uint8_t R_STS_WRITE_CMD    = 0x80;
        static const uint8_t R_TEMPERATURE_LSB  = 0x01;
        static const uint8_t R_TEMPERATURE_MSB  = 0x02;

        static const uint8_t B_CONT_READING     = 0x00;

        /**
         * Instantiates an MAX31723 object
         *
         * @param bus Number of the used bus
         * @param devAddr Address of the used I2C device
         */
        MAX31723 (int csn);

        /**
         * MAXDS3231M object destructor; basically, it closes the I2C connection.
         * it is not needed anymore, as the connections will be closed when
         * m_spi and m_csnPinCtx variables will go out of scope
         * ~MAX31723 ();
         **/

        /**
         * Gets the on-board temperature.
         */
        short getTemperature ();

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

        uint8_t readRegister (uint8_t reg);
        void writeRegister (uint8_t reg, uint8_t data);

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
