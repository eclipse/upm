/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Contributions: Jon Trulson <jtlulson@ics.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Credits to Seeed Studeo.
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
#include <mraa/i2c.hpp>

#define TH02_ADDR                0x40 // device address

#define TH02_REG_STATUS          0x00
#define TH02_REG_DATA_H          0x01
#define TH02_REG_DATA_L          0x02
#define TH02_REG_CONFIG          0x03
#define TH02_REG_ID              0x11

#define TH02_STATUS_RDY_MASK     0x01

#define TH02_CMD_MEASURE_HUMI    0x01
#define TH02_CMD_MEASURE_TEMP    0x11

namespace upm {

/**
 * @brief TH02 Temperature & Humidity Sensor library
 * @defgroup th02 libupm-th02
 * @ingroup seeed i2c temp
 */
/**
 * @library th02
 * @sensor th02
 * @comname TH02 Temperature & Humidity Sensor
 * @altname Grove Temperature & Humidity Sensor (High-Accuracy & Mini)
 * @type temp
 * @man seeed
 * @web http://www.seeedstudio.com/wiki/Grove_-_Tempture%26Humidity_Sensor_(High-Accuracy_%26Mini)_v1.0
 * @con i2c
 *
 * @brief API for the TH02 Temperature & Humidity Sensor
 * 
 *   This module defines the TH02 interface for libth02 
 *
 *   Note: For use on Intel(R) Edison with an Arduino* breakout board, Intel
 *   Edison must be set to 3 V rather than 5 V.
 *
 * @image html th02.jpg
 * @snippet th02.cxx Interesting
 */
class TH02 {
    public:
        /**
         * Instantiates a TH02 object
         */
        TH02 (int bus=0, uint8_t addr=TH02_ADDR);

        /**
         * TH02 object destructor; basically, it closes the I2C connection.
         */
        ~TH02 ();

        /**
         * Gets the temperature value from the sensor.
         */
        float getTemperature ();

        /**
         * Gets the humidity value from the sensor.
         */
        float getHumidity ();

        /**
         * Gets the sensor status.
         */
        bool getStatus ();

        /**
         * Returns the name of the component
         */
        std::string name()
        {
            return m_name;
        }

    private:
        std::string m_name;
        mraa::I2c m_i2c;
        uint8_t m_addr;
};

}
