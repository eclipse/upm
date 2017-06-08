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

#include <mraa/i2c.hpp>

#include "iTemperatureSensor.hpp"
#include "iHumiditySensor.hpp"
#include "iMraa.hpp"

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
class SI7005 :
    public virtual iMraa,
    public virtual iTemperatureSensor,
    public virtual iHumiditySensor {
    public:
        SI7005(const std::string &init_string);

        /**
         * Instantiates a SI7005 object
         *
         * @param pin gpio number for chip select pin
         * @param bus I2c bus number
         * @param addr (optional) I2c address, defaults to SI7005_ADDRESS
         */
        SI7005(int pin, int bus, int addr = SI7005_ADDRESS):
            SI7005("g:" + std::to_string(pin) + ":out," +
                   "i:" + std::to_string(bus) + ":" + std::to_string(addr)) {}

        virtual ~SI7005() {}

        /** Return the name of this device */
        virtual std::string Name () {return "SI7005";}

        /** Return the description of this device */
        virtual std::string Description () {return "Digital I2C humidity and temperature sensor";}

        /* Provide an implementation of a method to get sensor values by source */
        virtual std::map<std::string, float> TemperatureForSources(std::vector<std::string> sources);

        /* Provide an implementation of a method to get sensor values by source */
        virtual std::map<std::string, float> HumidityForSources(std::vector<std::string> sources);

        /**
         * Get temperature measurement.
         */
        uint16_t getTemperatureRaw ();

        /**
         * Get temperature measurement.
         */
        int getTemperatureCelsius ();

        /**
         * Get relative humidity measurement.
         */
        uint16_t getHumidityRaw ();

        /**
         * Get relative humidity measurement.
         */
        int getHumidityRelative ();

        /**
         * Reads from the device ID register as a means to verify the
         * sensor is connected.
         *
         * @return true if sensor ID register read succeeds, false otherwise
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

        mraa::Result status;
        uint8_t config_reg;
        float last_temperature;

        uint16_t getMeasurement(uint8_t configValue);
};

}
