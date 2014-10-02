/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Credits to Adafruit.
 * Based on Adafruit BMP085 library.
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
#include <math.h>

#define ADDR               0x77 // device address

// registers address
#define BMP085_ULTRALOWPOWER 0
#define BMP085_STANDARD      1
#define BMP085_HIGHRES       2
#define BMP085_ULTRAHIGHRES  3
#define BMP085_CAL_AC1           0xAA  // R   Calibration data (16 bits)
#define BMP085_CAL_AC2           0xAC  // R   Calibration data (16 bits)
#define BMP085_CAL_AC3           0xAE  // R   Calibration data (16 bits)
#define BMP085_CAL_AC4           0xB0  // R   Calibration data (16 bits)
#define BMP085_CAL_AC5           0xB2  // R   Calibration data (16 bits)
#define BMP085_CAL_AC6           0xB4  // R   Calibration data (16 bits)
#define BMP085_CAL_B1            0xB6  // R   Calibration data (16 bits)
#define BMP085_CAL_B2            0xB8  // R   Calibration data (16 bits)
#define BMP085_CAL_MB            0xBA  // R   Calibration data (16 bits)
#define BMP085_CAL_MC            0xBC  // R   Calibration data (16 bits)
#define BMP085_CAL_MD            0xBE  // R   Calibration data (16 bits)

#define BMP085_CONTROL           0xF4
#define BMP085_TEMPDATA          0xF6
#define BMP085_PRESSUREDATA      0xF6
#define BMP085_READTEMPCMD       0x2E
#define BMP085_READPRESSURECMD   0x34

#define HIGH               1
#define LOW                0

namespace upm {

/**
 * @brief GY65 & BPM085 atmospheric pressure sensor library
 * @defgroup gy65 libupm-gy65
 */

/**
 * @brief C++ API for GY65/BMP085 chip (Atmospheric Pressure Sensor)
 *
 * The Bosch [BMP085]
 * (https://www.sparkfun.com/datasheets/Components/General/BST-BMP085-DS000-05.pdf)
 * is a high precision, ultra-low power consumption pressure sensor. It has a
 * range of between 30,000 and 110,000 Pa.
 *
 * @ingroup gy65 i2c
 * @snippet gy65.cxx Interesting
 * @image html bmp085.jpeg
 */
class GY65 {
    public:
        /**
         * Instanciates a GY65 object
         *
         * @param bus number of used bus
         * @param devAddr address of used i2c device
         * @param mode BMP085 mode
         */
        GY65 (int bus, int devAddr=0x77, uint8_t mode=BMP085_ULTRAHIGHRES);

        /**
         * GY65 object destructor, basicaly it close i2c connection.
         */
        ~GY65 ();

        /**
         * Return calculated pressure
         */
        int32_t getPressure ();

        /**
         *
         * Get raw pressure data
         */
        int32_t getPressureRaw ();

        /**
         * Get raw temperature data from chip
         */
        int16_t getTemperatureRaw ();

        /**
         * Return calculated temperature
         */
        float getTemperature ();

        /**
         * With given absolute altitude sea level can be calculated
         *
         * @param altitudeMeters altitude
         */
        int32_t getSealevelPressure(float altitudeMeters = 0);

        /**
         * With given sea level altitude in meters can be calculated
         *
         * @param sealevelPressure Sea level
         */
        float getAltitude (float sealevelPressure = 101325);

        /**
         * Calculation of B5 (check spec for more information)
         *
         * @param UT
         */
        int32_t computeB5 (int32_t UT);

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

        /**
         * Read one byte register
         *
         * @param reg address of a register
         */
        uint8_t i2cReadReg_8 (int reg);

    private:
        std::string m_name;

        int m_controlAddr;
        int m_bus;
        mraa_i2c_context m_i2ControlCtx;

        uint8_t oversampling;
        int16_t ac1, ac2, ac3, b1, b2, mb, mc, md;
        uint16_t ac4, ac5, ac6;
};

}
