/*
 * Author: Scott Ware <scott.r.ware@intel.com>
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

#include "upm/iTemperatureHumiditySensor.h"

/* ADDRESS AND NOT_FOUND VALUE */
#define SI7005_ADDRESS                     ( 0x40 )
#define SI7005_NOT_FOUND                   ( 0x00 )

/* I2C BUS */
#define SI7005_I2C_BUS                     ( 1 )

/* CS PIN */
#define SI7005_CS_PIN                      ( 20 )

/* REGISTER ADDRESSES */
#define SI7005_REG_STATUS                  ( 0x00 )
#define SI7005_REG_DATA_HIGH               ( 0x01 )
#define SI7005_REG_DATA_LOW                ( 0x02 )
#define SI7005_REG_CONFIG                  ( 0x03 )
#define SI7005_REG_ID                      ( 0x11 )

/* STATUS REGISTER */
#define SI7005_STATUS_NOT_READY            ( 0x01 )

/* CONFIG REGISTER */
#define SI7005_CONFIG_START                ( 0x01 )
#define SI7005_CONFIG_HEAT                 ( 0x02 )
#define SI7005_CONFIG_HUMIDITY             ( 0x00 )
#define SI7005_CONFIG_TEMPERATURE          ( 0x10 )
#define SI7005_CONFIG_FAST                 ( 0x20 )
#define SI7005_CONFIG_RESET                ( 0x00 )

/* ID REGISTER */
#define SI7005_ID                          ( 0x50 )

/* COEFFICIENTS */
#define SI7005_TEMPERATURE_OFFSET          ( 50 )
#define SI7005_TEMPERATURE_SLOPE           ( 32 )
#define SI7005_HUMIDITY_OFFSET             ( 24 )
#define SI7005_HUMIDITY_SLOPE              ( 16 )
#define A0                                 ( -4.7844 )
#define A1                                 ( 0.4008 )
#define A2                                 ( -0.00393 )
#define Q0                                 ( 0.1973 )
#define Q1                                 ( 0.00237 )

/* MISCELLANEOUS */
#define SI7005_WAKE_UP_TIME                ( 15000 )

namespace upm {

/**
 * @brief Si7005 digital I2C humidity and temperature sensor library
 * @defgroup si7005 libupm-si7005
 */

/**
 * @brief C++ API for SI7005 sensor (Humidity And Temperature Sensor)
 *
 * The Silicon Labs Si7005
 * [SI7005](https://www.silabs.com/Support%20Documents/TechnicalDocs/Si7005.pdf)
 * is a digital relative humidity and temperature sensor.
 *
 * @ingroup Si7005 i2c
 * @snippet si7005.cxx Interesting
 */
class SI7005 : public ITemperatureHumiditySensor {
    public:
        /**
         * Instanciates a SI7005 object
         *
         * @param bus number of used bus
         * @param devAddr address of used i2c device
         * @param pin mraa gpio number for chip select pin
         */
        SI7005 (int bus = SI7005_I2C_BUS, int devAddr = SI7005_ADDRESS, int pin = SI7005_CS_PIN);

        /**
         * SI7005 object destructor.
         */
        ~SI7005 ();

        /**
         * Get temperature measurement.
         */
        mraa_result_t getTemperature (float* value);

        /**
         * Get relative humidity measurement.
         */
        mraa_result_t getHumidity (float* value);
	
        /**
         * Returns whether the sensor is configured.
         */
        bool isConfigured();

        /**
         * Detects the sensor to ensure it is connected as required.
         */
        bool isAvailable();

        /**
         * Enables the integrated heating element.
         */
        void enableHeater();

        /**
         * Disables the integrated heating element.
         */
        void disableHeater();

        /**
         * Enables fast conversion mode.
         */
        void enableFastConversionMode();

        /**
         * Enables fast conversion mode.
         */
        void disableFastConversionMode();

    private:
        int m_controlAddr;
        int m_bus;
        int m_pin;
        mraa_i2c_context m_i2cControlCtx;
        bool configured;
        uint8_t config_reg;
        float last_temperature;

        int getMeasurement(uint8_t configValue);
};

}
