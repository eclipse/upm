/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016-2017 Intel Corporation.
 *
 * The MIT License
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
#include "interfaces/iHumiditySensor.hpp"

#include "bmp280.hpp"

namespace upm {

    /**
     * @library bmp280
     * @sensor bme280
     * @comname Digital Humidity, Pressure, and Temperature Sensor
     * @type pressure humidity
     * @man bosch adafruit
     * @con i2c spi gpio
     * @web https://www.adafruit.com/products/2652
     *
     * @brief API for the BME280 Digital Humidity, Pressure, and
     * Temperature Sensor
     *
     * The BME280 is as combined digital humidity, pressure and
     * temperature sensor based on proven sensing principles. The
     * sensor module is housed in an extremely compact metal-lid LGA
     * package with a footprint of only 2.5 * 2.5 mm2 with a height of
     * 0.93 mm. Its small dimensions and its low power consumption
     * allow the implementation in battery driven devices such as
     * handsets, GPS modules or watches. The BME280 is register and
     * performance compatible to the Bosch Sensortec BMP280 digital
     * pressure sensor.
     *
     * @snippet bmp280-bme280.cxx Interesting
     */

    class BME280 : public BMP280, public IHumiditySensor {
    public:

        /**
         * BME280 constructor.
         *
         * This driver supports both the BMP280 and the BME280.  The
         * BME280 adds a humidity sensor.  The device type is detected
         * automatically by querying the chip id register.
         *
         * This device can support both I2C and SPI. For SPI, set the addr
         * to -1, and specify a positive integer representing the Chip
         * Select (CS) pin for the cs argument.  If you are using a
         * hardware CS pin, then you can connect the proper pin to the
         * hardware CS pin on your MCU and supply -1 for cs.  The default
         * operating mode is I2C.
         *
         * @param bus I2C or SPI bus to use.
         * @param addr The I2C address for this device.  Use -1 for SPI.
         * @param cs The gpio pin to use for the SPI Chip Select.  Use -1
         * for I2C, or for SPI with a hardware controlled pin.
         * @throws std::runtime_error on failure.
         */
        BME280(int bus=BME280_DEFAULT_I2C_BUS, int addr=BME280_DEFAULT_ADDR,
               int cs=-1);

        /**
         * BME280 Destructor.
         */
        virtual ~BME280();

        /**
         * Return the current measured relative humidity.  update()
         * must have been called prior to calling this method.  If the
         * device is a bmp280 rather than a bme280, this method will
         * always return 0.
         *
         * @return The relative humidity in percent.
         */
        float getHumidity();

        /**
         * Set the humidity sensor oversampling parameter.  See the data
         * sheet for details.  This value can be automatically set to a
         * suitable value by using one of the predefined modes for
         * setUsageMode().
         *
         * @param rate One of the OSRS_H_T values.
         */
        void setOversampleRateHumidity(BME280_OSRS_H_T rate);

        // Interface support
        const char *getModuleName()
        {
            return "BME280";
        };

        int getHumidityRelative()
        {
            return int(getHumidity());
        };

    protected:

    private:
    };
}
