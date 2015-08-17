/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
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
#include <mraa/spi.h>
#include <mraa/gpio.h>

namespace upm {

/**
 * @brief MAX31855 Thermocouple library
 * @defgroup max31855 libupm-max31855
 * @ingroup maxim adafruit spi other
 */
/**
 * @library max31855
 * @sensor max31855
 * @comname MAX31855 Thermocouple Amplifier
 * @type other
 * @man maxim adafruit
 * @con spi
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
