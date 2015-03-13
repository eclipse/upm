/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
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
#include <mraa/i2c.h>

#define ADDR                0x40 // device address

#define REG_STATUS          0x00
#define REG_DATA_H          0x01
#define REG_DATA_L          0x02
#define REG_CONFIG          0x03
#define REG_ID              0x11

#define STATUS_RDY_MASK     0x01

#define CMD_MEASURE_HUMI    0x01
#define CMD_MEASURE_TEMP    0x11

#define TH02_WR_REG_MODE    0xC0
#define TH02_RD_REG_MODE    0x80

#define HIGH                1
#define LOW                 0

namespace upm {

/**
 * @brief th02 temperature & humidity sensor library
 * @defgroup th02 libupm-th02
 */

/**
 * @brief C++ API for TH02 chip (Temperature and Humidity Sensor Pro)
 *
 * This file defines the TH02 C++ interface for libth02
 *
 * @ingroup th02
 * @snippet th02.cxx Interesting
 */
class TH02 {
    public:
        /**
         * Instanciates a TH02 object
         */
        TH02 ();

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
        mraa_i2c_context m_i2Ctx;

        uint16_t i2cReadReg_N (int reg, unsigned int len, uint8_t * buffer);
        mraa_result_t i2cWriteReg_N (uint8_t reg, unsigned int len, uint8_t * buffer);
        mraa_result_t i2cWriteReg (uint8_t reg, uint8_t data);
};

}
