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
#include <interfaces/iTemperature.hpp>

#define HIGH                    1
#define LOW                     0

namespace upm {

/**
 * @brief MAX31723 Temperature Sensor
 * @defgroup max31723 libupm-max31723
 * @ingroup maxim spi temp
 */
/**
 * @library max31723
 * @sensor max31723
 * @comname SPI/3-wire Digital Thermometer
 * @type temp
 * @man maxim
 * @con spi
 * @web https://www.maximintegrated.com/en/products/analog/sensors-and-sensor-interface/MAX31723.html
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
class MAX31723 : virtual public iTemperature {
    public:
        static const uint8_t R_STS_READ_CMD     = 0x00;
        static const uint8_t R_STS_WRITE_CMD    = 0x80;
        static const uint8_t R_TEMPERATURE_LSB  = 0x01;
        static const uint8_t R_TEMPERATURE_MSB  = 0x02;

        static const uint8_t B_CONT_READING     = 0x00;

        /**
         * Instantiates an MAX31723 object
         *
         * @param bus Number of the SPI bus used
         * @param csn Chip select (slave select) pin to use
         */
        MAX31723 (int bus, int csn);

        /**
         * MAXDS3231M object destructor; basically, it closes the I2C connection.
         * it is not needed anymore, as the connections will be closed when
         * m_spi and m_csnPinCtx variables will go out of scope
         * ~MAX31723 ();
         **/

        /**
         * Returns the temperature in degrees Celsius
         *
         * @return The Temperature in degrees Celsius
         */
        virtual float getTemperature ();

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
