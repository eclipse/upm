/*
 * Author: Scott Ware <scott.r.ware@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <mraa/i2c.hpp>

#include <interfaces/iTemperature.hpp>
#include <interfaces/iHumidity.hpp>

/* ADDRESS AND NOT_FOUND VALUE */
#define SI7005_ADDRESS                     ( 0x40 )
#define SI7005_NOT_FOUND                   ( 0x00 )

namespace upm {

/**
 * @brief SI7005 Digital Humidity and Temperature Sensor
 * @defgroup si7005 libupm-si7005
 * @ingroup i2c silabs temp itemperaturesensor
 */

/**
 * @library si7005
 * @sensor si7005
 * @comname Digital I2C Humidity and Temperature Sensor
 * @altname Si7005
 * @type temp
 * @man silabs
 * @web http://www.silabs.com/products/sensors/humidity-sensors/Pages/Si7005.aspx
 * @con i2c
 * @if itemperaturesensor
 *
 * @brief C++ API for SI7005 sensor (Humidity And Temperature Sensor)
 *
 * The Silicon Labs Si7005
 * [SI7005](https://www.silabs.com/Support%20Documents/TechnicalDocs/Si7005.pdf)
 * is a digital relative humidity and temperature sensor.
 *
 * @snippet si7005.cxx Interesting
 */
class SI7005 : virtual public iTemperature, virtual public iHumidity {
    public:
        /**
         * Instantiates a SI7005 object
         *
         * @param bus number of used bus
         * @param pin gpio number for chip select pin
         */
        SI7005 (int bus, int pin);

        /**
         * SI7005 object destructor.
         */
        ~SI7005 ();

        /**
         * Get temperature measurement.
         */
        uint16_t getTemperatureRaw ();

        /**
         * Get temperature measurement.
         */
        int getTemperatureCelsius ();

        /**
         * Get the current temperature.
         *
         * @return The last temperature reading in Celsius
         */
        virtual float getTemperature();

        /**
         * Get relative humidity measurement.
         */
        uint16_t getHumidityRaw ();

        /**
         * Get relative humidity measurement.
         */
        int getHumidityRelative ();

        /**
         * Get the current relative humidity.
         *
         * @return The last humidity reading
         */
        virtual float getHumidity();

        /**
         * Returns sensor module name
         */
        virtual const char* getModuleName() { return "si7005"; }

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
        /* Disable implicit copy and assignment operators */
        SI7005(const SI7005&) = delete;
        SI7005 &operator=(const SI7005&) = delete;

        int m_controlAddr;
        int m_bus;
        int m_pin;
        mraa::I2c* m_i2c;
        mraa::Result status;
        uint8_t config_reg;
        float last_temperature;

        uint16_t getMeasurement(uint8_t configValue);
};

}
