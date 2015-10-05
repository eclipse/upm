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
#include <mraa/i2c.hpp>

#define ADDR               0x4A // device address

// registers address
#define ISR                0x00 // Interrupt Status Register
#define MCR                0x01 // Main Configuration Register
#define RCR                0x02 // Receive Configuration Register
#define TCR                0x03 // Transmit Configuration Register
#define ALSDATA_HIGH       0x04 // ambient sensor data high byte
#define ALSDATA_LOW        0x05 // ambient sensor data low byte
#define PRXDATA            0x15 // proximity sensor data

#define ALS_UP_THRESH_HIGH 0x06 // ALS Interrupt Threshold Registers High
#define ALS_UP_THRESH_LOW  0x07 // ALS Interrupt Threshold Registers Low
#define ALS_LO_THRESH_HIGH 0x08 // ALS Interrupt Threshold Registers High
#define ALS_LO_THRESH_LOW  0x09 // ALS Interrupt Threshold Registers Low
#define TPTR               0x0A // ALS/PROX Threshold Persist Timer Register
#define PROX_THRESH_IND    0x0B // Proximity Threshold Register
#define PROX_THRESH        0x0C // Proximity Threshold Register
#define TRIM_GAIN_GREEN    0x0F // Digital Gain Trim Register
#define TRIM_GAIN_IR       0x10 // Digital Gain Trim Register

#define HIGH               1
#define LOW                0

namespace upm {

/**
 * @brief MAX44000 Proximity Sensor library
 * @defgroup max44000 libupm-max44000
 * @ingroup maxim i2c light
 */
/**
 * @library max44000
 * @sensor max44000
 * @comname MAX44000 Proximity Sensor
 * @type light
 * @man maxim
 * @con i2c
 *
 * @brief API for the MAX44000 Ambient and Infrared Proximity Sensor
 *
 * Maxim Integrated*
 * [MAX44000](http://datasheets.maximintegrated.com/en/ds/MAX44000.pdf)
 * is an ambient and infrared proximity sensor. This module was tested on the
 * Maxim Integrated 
 * [MAX44000PMB1 PMOD module]
 * (http://datasheets.maximintegrated.com/en/ds/MAX44000PMB1.pdf) from the
 * analog PMOD kit.
 *
 * @snippet max44000.cxx Interesting
 */
class MAX44000 {
    public:
        /**
         * Instantiates an MAX44000 object
         *
         * @param bus Number of the used bus
         * @param devAddr Address of the used I2C device
         */
        MAX44000 (int bus, int devAddr=ADDR);

        /**
         * MAX44000 object destructor; basically, it closes the I2C connection.
         * ~MAX44000 ();
         * no need for the destructor - the I2c connection will be closed when
         * m_i2cMaxControlCtx variable will be out of context
         **/

        /**
         * Reads the proximity value from the sensor (based on ambient data).
         */
        uint16_t getProximity ();
        /**
         * Reads the ambient value from the sensor (based on ambient data).
         */
        uint16_t getAmbient ();

        /**
         * Returns the name of the component
         */
        std::string name()
        {
            return m_name;
        }

        /**
         * Reads a one-byte register
         *
         * @param reg Address of the register
         */
        uint8_t i2cReadReg_8 (int reg);

        /**
         * Reads a two-byte register
         *
         * @param reg Address of the register
         */
        uint16_t i2cReadReg_16 (int reg);

        /**
         * Writes to a one-byte register
         *
         * @param reg Address of the register
         * @param value Byte to be written
         */
        mraa::Result i2cWriteReg (uint8_t reg, uint8_t value);

    private:
        std::string m_name;

        int m_maxControlAddr;
        int m_bus;
        mraa::I2c m_i2cMaxControlCtx;
};

}
