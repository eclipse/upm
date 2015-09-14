/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Contributions: Jon Trulson <jtrulson@ics.com>
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
#include <mraa/i2c.hpp>
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
 * @brief Bosch BMP & GY65 Atmospheric Pressure Sensor library
 * @defgroup bmpx8x libupm-bmpx8x
 * @ingroup seeed adafruit sparkfun i2c pressure
 */

/**
 * @library bmpx8x
 * @sensor bmpx8x
 * @comname BMP Atmospheric Pressure Sensor
 * @altname GY65 BMP085 BMP180 BMP183
 * @type pressure
 * @man seeed adafruit sparkfun
 * @con i2c
 *
 * @brief API for the GY65/BMP085 and BMP180 Atmospheric Pressure Sensors
 *
 * Bosch GY65/BMP085 and BMP180 are high-precision, ultra-low
 * power consumption pressure sensors. They operate in the range of
 * 30,000-110,000 Pa.
 *
 * This module has been tested on the GY65/BMP085 and BMP180 sensors.
 *
 * @image html bmp085.jpeg
 * @snippet bmpx8x.cxx Interesting
 */

class BMPX8X {
    public:
        /**
         * Instantiates a BMPX8X object
         *
         * @param bus Number of the used bus
         * @param devAddr Address of the used I2C device
         * @param mode BMP085 mode
         */
        BMPX8X (int bus, int devAddr=0x77, uint8_t mode=BMP085_ULTRAHIGHRES);

        /**
         * BMPX8X object destructor; basically, it closes the I2C connection.
         * ~BMPX8X ();
         * LE: there is no need for the destructor, as the I2C connection
         * will be closed when the m_i2ControlCtx variable will go out of
         * scope (when all the BMPX8X objects will be destroyed)
         */
        /**
         * Returns the calculated pressure
         */
        int32_t getPressure ();

        /**
         *
         * Gets raw pressure data
         */
        int32_t getPressureRaw ();

        /**
         * Gets raw temperature data from the sensor
         */
        int16_t getTemperatureRaw ();

        /**
         * Returns the calculated temperature
         */
        float getTemperature ();

        /**
         * With a given absolute altitude, sea level can be calculated
         *
         * @param altitudeMeters Altitude
         */
        int32_t getSealevelPressure(float altitudeMeters = 0);

        /**
         * With a given sea level, altitude in meters can be calculated
         *
         * @param sealevelPressure Sea level
         */
        float getAltitude (float sealevelPressure = 101325);

        /**
         * Calculates B5 (check the spec for more information)
         *
         * @param UT
         */
        int32_t computeB5 (int32_t UT);

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

        /**
         * Reads a one-byte register
         *
         * @param reg Address of the register
         */
        uint8_t i2cReadReg_8 (int reg);

    private:
        std::string m_name;

        int m_controlAddr;
        int m_bus;
        mraa::I2c m_i2ControlCtx;

        uint8_t oversampling;
        int16_t ac1, ac2, ac3, b1, b2, mb, mc, md;
        uint16_t ac4, ac5, ac6;
};

}
