/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
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
#include <mraa/spi.h>
#include <mraa/gpio.h>

namespace upm {

/**
 * @brief MAX31855 Thermocouple
 * @defgroup max31855 libupm-max31855
 * @ingroup maxim adafruit spi other
 */
/**
 * @library max31855
 * @sensor max31855
 * @comname Thermocouple-to-Digital Converter
 * @type other
 * @man maxim adafruit
 * @con spi
 * @web https://www.maximintegrated.com/en/products/analog/sensors-and-sensor-interface/MAX31855.html
 *
 * @brief API for the MAX31855 Thermocouple Amplifier
 *
 * Maxim Integrated*
 * [MAX31855](http://datasheets.maximintegrated.com/en/ds/MAX31855.pdf)
 * is a cold-junction compensated thermocouple-to-digital converter. This
 * module was tested on the Maxim Integrated 
 * [MAX31855PMB1 PMOD module]
 * (http://datasheets.maximintegrated.com/en/ds/MAX31855PMB1.pdf) from the
 * analog PMOD kit.
 *
 * @image html max31855.jpg
 * @snippet max31855.cxx Interesting
 */
class MAX31855 {
    public:
        /**
         * Instantiates an MAX31855 object
         *
         * @param bus SPI bus to use
         * @param cs Chip select pin
         */
        MAX31855(int bus, int cs);

        /**
         * MAX31855 object destructor
         */
        ~MAX31855();

        /**
         * Gets the distance from the sensor
         *
         * @return Value in Celsius
         */
//! [Interesting]
        double getTemp();
//! [Interesting]

    private:
        mraa_spi_context m_sensor;
        mraa_gpio_context m_gpio;
};

}
