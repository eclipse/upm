/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Contributions: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Credits to Adafruit.
 * Based on Adafruit BMPX8X library.
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

#include "upm/iPressureSensor.h"

#define BMPX8X_ADDR               0x77 // device address

// REGISTERS
#define BMPX8X_ULTRALOWPOWER 0
#define BMPX8X_STANDARD      1
#define BMPX8X_HIGHRES       2
#define BMPX8X_ULTRAHIGHRES  3
#define BMPX8X_CAL_AC1           0xAA  // R   Calibration data (16 bits)
#define BMPX8X_CAL_AC2           0xAC  // R   Calibration data (16 bits)
#define BMPX8X_CAL_AC3           0xAE  // R   Calibration data (16 bits)
#define BMPX8X_CAL_AC4           0xB0  // R   Calibration data (16 bits)
#define BMPX8X_CAL_AC5           0xB2  // R   Calibration data (16 bits)
#define BMPX8X_CAL_AC6           0xB4  // R   Calibration data (16 bits)
#define BMPX8X_CAL_B1            0xB6  // R   Calibration data (16 bits)
#define BMPX8X_CAL_B2            0xB8  // R   Calibration data (16 bits)
#define BMPX8X_CAL_MB            0xBA  // R   Calibration data (16 bits)
#define BMPX8X_CAL_MC            0xBC  // R   Calibration data (16 bits)
#define BMPX8X_CAL_MD            0xBE  // R   Calibration data (16 bits)

#define BMPX8X_CHIP_ID           0xD0
#define BMPX8X_CONTROL           0xF4

#define BMPX8X_DATA_MSB          0xF6
#define BMPX8X_DATA_LSB          0xF7
#define BMPX8X_DATA_XLSB         0xF8

#define BMPX8X_READTEMPCMD       0x2E
#define BMPX8X_READPRESSURECMD   0x34

#define BMPX8X_ID                0x55

namespace upm {

/**
 * @brief GY65/BPMX8X atmospheric pressure sensor library
 * @defgroup bmpx8x libupm-bmpx8x
 * @ingroup seeed adafruit sparkfun i2c pressure
 */

/**
 * @library bmpx8x
 * @sensor bmpx8x
 * @comname Bosch BMP Atmospheric Pressure Sensor
 * @altname GY65 BMP085 BMP180 BMP183
 * @type pressure
 * @man seeed adafruit sparkfun
 * @con i2c
 *
 * @brief C++ API for GY65/BMPX8X chips (Atmospheric Pressure Sensor)
 *
 * The Bosch GY65/BMPX8X are high precision, ultra-low
 * power consumption pressure sensors. They have a range of between
 * 30,000 and 110,000 Pa.
 *
 * This module has been tested on the GY65/BMPX8X sensors.
 *
 * @image html bmpX8X.jpeg
 * @snippet bmpx8x.cxx Interesting
 */

class BMPX8X : public IPressureSensor {
    public:
        /**
         * Instanciates a BMPX8X object
         *
         * @param bus number of used bus
         * @param devAddr address of used i2c device
         * @param mode BMPX8X mode
         */
        BMPX8X (int bus, int devAddr=BMPX8X_ADDR, uint8_t mode=BMPX8X_ULTRAHIGHRES);

        /**
         * BMPX8X object destructor, basicaly it close i2c connection.
         */
        ~BMPX8X ();

        /**
         * Return calculated pressure (Pa)
         */
        mraa_result_t getPressure (int32_t *value);

        /**
         * Return latest calculated temperature value (0.1C)
         */
        int32_t getTemperature ();

        /**
         * With given absolute altitude sea level can be calculated
         *
         * @param altitudeMeters altitude
         */
        int32_t getSealevelPressure(int32_t pressure, int32_t altitudeMeters = 0);

        /**
         * With given sea level altitude in meters can be calculated
         *
         * @param sealevelPressure Sea level
         */
        int32_t getAltitude (int32_t pressure, int32_t sealevelPressure = 101325);

        /**
         * Returns whether the sensor is configured.
         */
        bool isConfigured();

        /**
         * Returns whether the correct chip is present at the given address.
         */
        bool isAvailable();

    private:
        std::string m_name;

        int m_controlAddr;
        int m_bus;
        mraa_i2c_context m_i2ControlCtx;

        uint8_t oversampling;
        int16_t ac1, ac2, ac3, b1, b2, mb, mc, md;
        uint16_t ac4, ac5, ac6;

        int32_t lastTemperature;

        bool configured;

        bool getCalibrationData();
        int32_t getPressureRaw ();
        int32_t getTemperatureRaw ();
};

}
