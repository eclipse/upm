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
#include <mraa/i2c.h>

#define ADDR               0x4A // device address

// registers address
#define ISR                0x00 // Interrupt Status Register
#define IE                 0x01 // Interrupt Enable Register
#define CONF               0x02 // Configuration Register
#define LUXDATA_HIGH       0x03 // Lux Reading High Byte
#define LUXDATA_LOW        0x04 // Lux Reading Low Byte

#define UP_THRESH_HIGH     0x05 // Upper Threshold Registers High
#define UP_THRESH_HIGH     0x06 // Lower Threshold Registers High
#define THRESH_TIMER       0x07 // Threshold Timer Register

#define HIGH               1
#define LOW                0

namespace upm {

/**
 * @brief max44009 proxmity sensor library
 * @defgroup max44009 libupm-max44009
 */

/**
 * @brief C++ API for MAX44009 chip (Ambient Light Sensor)
 *
 * The Maxim Integrated
 * [MAX44009](http://datasheets.maximintegrated.com/en/ds/MAX44009.pdf)
 * is Industry’s Lowest-Power Ambient Light Sensor with ADC.
 *
 * @ingroup max44009 i2c
 * @snippet max44009.cxx Interesting
 */
class MAX44009 {
    public:
        /**
         * Instanciates a MAX44009 object
         *
         * @param bus number of used bus
         * @param devAddr addres of used i2c device
         */
        MAX44009 (int bus, int devAddr);

        /**
         * MAX44009 object destructor, basicaly it close i2c connection.
         */
        ~MAX44009 ();

        /**
         * Read the lux value from the chip.
         */
        uint16_t getLux ();

        /**
         * Return name of the component
         */
        std::string name()
        {
            return m_name;
        }

        /**
         * Read one byte register
         *
         * @param reg address of a register
         */
        uint8_t i2cReadReg_8 (int reg);

        /**
         * Read two bytes register
         *
         * @param reg address of a register
         */
        uint16_t i2cReadReg_16 (int reg);

        /**
         * Write to one byte register
         *
         * @param reg address of a register
         * @param value byte to be written
         */
        mraa_result_t i2cWriteReg (uint8_t reg, uint8_t value);

    private:
        std::string m_name;

        int m_maxControlAddr;
        int m_bus;
        mraa_i2c_context m_i2cMaxControlCtx;
};

}
