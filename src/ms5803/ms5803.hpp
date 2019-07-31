/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <string>
#include <interfaces/iPressure.hpp>
#include <interfaces/iTemperature.hpp>

#include "ms5803.h"

namespace upm {

    /**
     * @brief MS5803 Pressure and Temperature
     * @defgroup ms5803 libupm-ms5803
     * @ingroup i2c spi gpio temp pressure
     */

    /**
     * @library ms5803
     * @sensor ms5803
     * @comname I2C Barometric Pressure and Temperature Sensor
     * @type temperature pressure
     * @man sparkfun
     * @con i2c spi
     * @web https://www.sparkfun.com/products/12909
     *
     * @brief UPM API for the MS5803 Pressure and Temperature sensor
     *
     * The MS5803-14BA is a new generation of high resolution pressure
     * sensors with SPI and I2C bus interface. It is optimized for
     * depth measurement systems with a water depth resolution of 1cm
     * and below. The sensor module includes a high linear pressure
     * sensor and an ultra low power 24 bit delta-sigma ADC with
     * internal factory calibrated coefficients. It provides a precise
     * digital 24 bit pressure and temperature value and different
     * operation modes that allow the user to optimize for conversion
     * speed and current consumption. A high resolution temperature
     * output allows the implementation of a depth measurement systems
     * and thermometer function without any additional sensor.
     *
     * The device is driven at 3.3vdc, and has a pressure range of
     * between 0 and 14 Bar, and a temperature range of between -40
     * and +85C.
     *
     * @snippet ms5803.cxx Interesting
     */
    class MS5803 : virtual public iPressure, virtual public iTemperature {
    public:

        /**
         * MS5803 constructor.  The default arguments inititialize I2C
         * operation and the default I2C address.
         *
         * @param bus i2c/spi bus to use
         * @param address The address for this device if using I2C.  If
         * using SPI, supply -1 for this parameter.
         * @param csPin The GPIO pin to use for Chip Select (CS).  This is
         * only needed for SPI, and only if your SPI implementation
         * requires it.  Otherwise, just pass -1 if not using SPI, or your
         * CS is handled automatically by your SPI implementation.
         */
        MS5803(int bus, int address=MS5803_DEFAULT_I2C_ADDR,
               int csPin=-1);

        /**
         * MS5803 Destructor
         */
        ~MS5803();

        /**
         * Reset the device.
         *
         */
        void reset();

        /**
         * Take a measurement and store the current sensor values
         * internally.  This function must be called prior to retrieving
         * any sensor values, for example getTemperature().
         *
         */
        void update();

        /**
         * Set the output sampling resolution of the temperature
         * measurement.  Higher values provide a more precise value.  In
         * addition, more precise values require more time to measure.
         * The default set at device intialization is the highest
         * precision supported: MS5803_OSR_4096
         *
         * @param osr One of the MS5803_OSR_T values.
         */
        void setTemperatureOSR(MS5803_OSR_T osr);

        /**
         * Set the output sampling resolution of the pressure
         * measurement.  Higher values provide a more precise value.  In
         * addition, more precise values require more time to measure.
         * The default set at device intialization is the highest
         * precision supported: MS5803_OSR_4096
         *
         * @param osr One of the MS5803_OSR_T values.
         */
        void setPressureOSR(MS5803_OSR_T osr);

        /**
         * Return the latest measured temperature.  update() must have
         * been called prior to calling this function.  The returned
         * value is in degrees Celsius.
         *
         * @return Temperature in degrees C
         */
        virtual float getTemperature();

        /**
         * Return the latest measured pressure.  update() must have
         * been called prior to calling this function.  The returned
         * value is in pascal.
         *
         * @return Pressure in Pa
         */
        virtual float getPressure();

    protected:
        ms5803_context m_ms5803;

        /**
         * Load a series of factory installed compensation coefficients.
         * This function is called during ms5803_init(), so it should
         * never need to be called again.  It is provided here anyway
         * "just in case".
         *
         * @param dev Device context.
         * @return UPM Status.
         */
        void loadCoefficients();

        /**
         * Perform a bus read.  This function is bus agnostic.  It is
         * exposed here for those users wishing to perform their own low
         * level accesses.  This is a low level function, and should not
         * be used unless you know what you are doing.
         *
         * @param cmd The command or register to access.
         * @param cnt The number of bytes to read.
         * @return The data read
         */
        std::string busRead(int cmd, int len);

        /**
         * Perform a bus write.  This function is bus agnostic.  It is
         * exposed here for those users wishing to perform their own low
         * level accesses.  This is a low level function, and should not
         * be used unless you know what you are doing.
         *
         * @param cmd The command or register to access.
         * @param data The string containing the data to write
         */
        void busWrite(int cmd, std::string data);

    private:
        /* Disable implicit copy and assignment operators */
        MS5803(const MS5803&) = delete;
        MS5803 &operator=(const MS5803&) = delete;
    };
}
