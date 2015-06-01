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
 * @brief API for th02 temperature & humidity sensor library
 *
 *   This file defines the TH02 interface for libth02
 *
 * @defgroup th02 libupm-th02
 * @ingroup seeed i2c temp
 */
/**
 * @library th02
 * @sensor th02
 * @comname temperature and humidity sensor
 * @type temp
 * @man seeed
 * @web http://www.seeedstudio.com/wiki/Grove_-_Tempture%26Humidity_Sensor_(High-Accuracy_%26Mini)_v1.0
 * @con i2c
 *
 * @brief API for th02 temperature & humidity sensor library
 * 
 *   This file defines the TH02 interface for libth02 
 *
 *   NOTE: For use on the Edison with the arduino breakout board, the
 *   Edison must be set to 3v rather than 5v.
 *
 * @image html th02.jpg
 * @snippet th02.cxx Interesting
 */
class TH02 {
    public:
        /**
         * Instanciates a TH02 object
         */
        TH02 (int bus=0, uint8_t addr=TH02_ADDR);

        /**
         * TH02 object destructor, basicaly it close i2c connection.
         */
        ~TH02 ();

        /**
         * Get the temperature value from sensor.
         */
        float getTemperature ();

        /**
         * Get the humidity value from sensor.
         */
        float getHumidity ();

        /**
         * Get the sensor's status.
         */
        bool getStatus ();

        /**
         * Return name of the component
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
