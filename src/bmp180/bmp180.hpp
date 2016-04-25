/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Contributions: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Credits to Adafruit.
 * Based on Adafruit BMP180 library.
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
#include "upm/iPressureSensor.hpp"
#include "upm/iTemperatureSensor.hpp"

#define BMP180_ADDR               0x77 // device address

/* MODES */
#define BMP180_ULTRALOWPOWER 0
#define BMP180_STANDARD      1
#define BMP180_HIGHRES       2
#define BMP180_ULTRAHIGHRES  3


namespace upm {

/**
 * @brief BPM180 atmospheric pressure sensor library
 * @defgroup BMP180 libupm-BMP180
 * @ingroup bosch seeed adafruit sparkfun i2c pressure ipressuresensor
 */

/**
 * @brief API for Bosch BMP180 Pressure Sensor
 *
 * The Bosch BMP180 is a high precision, ultra-low
 * power consumption pressure sensor. It has a range of between
 * 30,000 and 110,000 Pa.
 *
 * @library BMP180
 * @sensor BMP180
 * @comname Bosch BMP Atmospheric Pressure Sensor
 * @altname BMP180
 * @type pressure
 * @man bosch adafruit sparkfun
 * @con i2c
 * @if ipressuresensor
 *

 */

class BMP180 : public IPressureSensor, public ITemperatureSensor {
    public:
        /**
         * Instanciates a BMP180 object
         *
         * @param bus number of used bus
         * @param devAddr address of used i2c device
         * @param mode BMP180 mode
         */
        BMP180 (int bus, int devAddr=BMP180_ADDR, uint8_t mode=BMP180_ULTRAHIGHRES);

        /**
         * BMP180 object destructor, basicaly it close i2c connection.
         */
        ~BMP180 ();

        /**
         * Return pressure
         */
        uint32_t getPressureRaw();

        /**
         * Return calculated pressure (Pa)
         */
        int getPressurePa();

        uint16_t getTemperatureRaw();
        /**
         * Return latest calculated temperature value in Celcius
         */
        int getTemperatureCelcius();

        /**
         * Returns whether the correct chip is present at the given address.
         */
        bool isAvailable();
        const char* getModuleName() { return m_name.c_str(); }

    private:
        std::string m_name;

        int m_controlAddr;
        int m_bus;
        mraa::I2c* m_i2c;
        mraa::Result status;

        uint8_t oversampling;
        int16_t ac1, ac2, ac3, b1, b2, mb, mc, md;
        uint16_t ac4, ac5, ac6;

        int32_t b5;

        bool getCalibrationData();
};

}
