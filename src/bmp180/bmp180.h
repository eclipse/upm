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
#include <mraa/i2c.h>
#include <math.h>

#include "upm/iPressureSensor.h"

#define BMP180_ADDR               0x77 // device address

/* MODES */
#define BMP180_ULTRALOWPOWER 0
#define BMP180_STANDARD      1
#define BMP180_HIGHRES       2
#define BMP180_ULTRAHIGHRES  3

/* CALIBRATION */
#define BMP180_PROM_START_ADDR          (0xAA)
#define BMP180_PROM_DATA_LEN            (22)

#define	BMP180_CALIB_DATA_SIZE			(22)
#define	BMP180_CALIB_PARAM_AC1_MSB		(0)
#define	BMP180_CALIB_PARAM_AC1_LSB		(1)
#define	BMP180_CALIB_PARAM_AC2_MSB		(2)
#define	BMP180_CALIB_PARAM_AC2_LSB		(3)
#define	BMP180_CALIB_PARAM_AC3_MSB		(4)
#define	BMP180_CALIB_PARAM_AC3_LSB		(5)
#define	BMP180_CALIB_PARAM_AC4_MSB		(6)
#define	BMP180_CALIB_PARAM_AC4_LSB		(7)
#define	BMP180_CALIB_PARAM_AC5_MSB		(8)
#define	BMP180_CALIB_PARAM_AC5_LSB		(9)
#define	BMP180_CALIB_PARAM_AC6_MSB		(10)
#define	BMP180_CALIB_PARAM_AC6_LSB		(11)
#define	BMP180_CALIB_PARAM_B1_MSB		(12)
#define	BMP180_CALIB_PARAM_B1_LSB		(13)
#define	BMP180_CALIB_PARAM_B2_MSB		(14)
#define	BMP180_CALIB_PARAM_B2_LSB		(15)
#define	BMP180_CALIB_PARAM_MB_MSB		(16)
#define	BMP180_CALIB_PARAM_MB_LSB		(17)
#define	BMP180_CALIB_PARAM_MC_MSB		(18)
#define	BMP180_CALIB_PARAM_MC_LSB		(19)
#define	BMP180_CALIB_PARAM_MD_MSB		(20)
#define	BMP180_CALIB_PARAM_MD_LSB		(21)

/* REGISTERS */
#define BMP180_TEMPERATURE_DATA_LENGTH			((uint8_t)2)
#define BMP180_PRESSURE_DATA_LENGTH				((uint8_t)3)

#define BMP180_CHIP_ID_REG			(0xD0)
#define BMP180_VERSION_REG			(0xD1)

#define BMP180_CTRL_MEAS_REG		(0xF4)
#define BMP180_ADC_OUT_MSB_REG		(0xF6)
#define BMP180_ADC_OUT_LSB_REG		(0xF7)

#define BMP180_SOFT_RESET_REG		(0xE0)

/* temperature measurement */
#define BMP180_T_MEASURE			(0x2E)

/* pressure measurement*/
#define BMP180_P_MEASURE			(0x34)

#define	BMP180_TEMPERATURE_DATA_BYTES	(2)
#define	BMP180_PRESSURE_DATA_BYTES		(3)
#define	BMP180_TEMPERATURE_LSB_DATA		(1)
#define	BMP180_TEMPERATURE_MSB_DATA		(0)
#define	BMP180_PRESSURE_MSB_DATA		(0)
#define	BMP180_PRESSURE_LSB_DATA		(1)
#define	BMP180_PRESSURE_XLSB_DATA		(2)

#define BMP180_PARAM_MG		(3038)
#define BMP180_PARAM_MH		(-7357)
#define BMP180_PARAM_MI		(3791)

/* ID */
#define BMP180_ID                (0x55)

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

class BMP180 : public IPressureSensor {
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
         * Return calculated pressure (Pa)
         */
        mraa_result_t getPressure (int32_t *value);

        /**
         * Return latest calculated temperature value (0.1C)
         */
        mraa_result_t getTemperature (int16_t* value);

        /**
         * Returns whether the sensor is configured.
         */
        bool isConfigured();

        /**
         * Returns whether the correct chip is present at the given address.
         */
        bool isAvailable();
        const char* getModuleName() { return "bmp180"; }

    private:
        std::string m_name;

        int m_controlAddr;
        int m_bus;
        mraa_i2c_context m_i2ControlCtx;

        uint8_t oversampling;
        int16_t ac1, ac2, ac3, b1, b2, mb, mc, md;
        uint16_t ac4, ac5, ac6;

        int32_t b5;

        bool configured;

        bool getCalibrationData();
        uint32_t getPressureRaw ();
        uint16_t getTemperatureRaw ();
};

}
