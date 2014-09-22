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
 * @brief max31723 temperature sensor library
 * @defgroup max31723 libupm-max31723
 */

/**
 * @brief C++ API for MAX31723 chip (Temperature sensor)
 *
 * The Maxim Integrated
 * [MAX31723](http://datasheets.maximintegrated.com/en/ds/MAX31722-MAX31723.pdf)
 * is a Low-Voltage 3-Wire/SPI Temperature Sensor controller. This module was
 * tested on the Maxim Integrated [MAX31732PMB1 PMOD
 * module](http://datasheets.maximintegrated.com/en/ds/MAX31723PMB1.pdf) from
 * the analog PMOD kit.
 *
 * @ingroup max31723
 * @snippet max31723.cxx Interesting
 * @image html max31723.jpeg
 */
class MAX31723 {
    public:
        static const uint8_t R_STS_READ_CMD     = 0x00;
        static const uint8_t R_STS_WRITE_CMD    = 0x80;
        static const uint8_t R_TEMPERATURE_LSB  = 0x01;
        static const uint8_t R_TEMPERATURE_MSB  = 0x02;

        static const uint8_t B_CONT_READING     = 0x00;

        /**
         * Instanciates a MAX31723 object
         *
         * @param bus number of used bus
         * @param devAddr addres of used i2c device
         */
        MAX31723 (int csn);

        /**
         * MAXDS3231M object destructor, basicaly it close i2c connection.
         */
        ~MAX31723 ();

        /**
         * Get on board temperature.
         */
        short getTemperature ();

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

        uint8_t readRegister (uint8_t reg);
        void writeRegister (uint8_t reg, uint8_t data);

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
