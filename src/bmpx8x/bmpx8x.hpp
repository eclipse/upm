/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * This driver was rewritten based on the original driver written by:
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 *
 * The MIT License
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <string>

#include "bmpx8x.h"

#include "mraa/initio.hpp"
#include <interfaces/iPressure.hpp>
#include <interfaces/iTemperature.hpp>

namespace upm {

    /**
     * @brief BMP & GY65 Atmospheric Pressure Sensors
     * @defgroup bmpx8x libupm-bmpx8x
     * @ingroup bosch seeed adafruit sparkfun i2c
     * pressure
     */

    /**
     * @library bmpx8x
     * @sensor bmpx8x
     * @comname BMP Atmospheric Pressure Sensor
     * @altname GY65 BMP085 BMP180 BMP183
     * @type pressure
     * @man bosch seeed adafruit sparkfun
     * @con i2c
     * @web https://www.sparkfun.com/datasheets/Components/General/BST-BMP085-DS000-05.pdf
     * @web https://www.bosch-sensortec.com/bst/products/all_products/bmp180
     * @web https://cdn-shop.adafruit.com/datasheets/1900_BMP183.pdf
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

    class BMPX8X : virtual public iPressure, virtual public iTemperature {
    public:
        /**
         * Instantiates a BMPX8X object
         *
         * @param bus I2C bus to use.
         * @param addr The I2C address of the device.
         * @throws std::runtime_error on failure.
         */
        BMPX8X(int bus=BMPX8X_DEFAULT_I2C_BUS,
               int addr=BMPX8X_DEFAULT_I2C_ADDR);

        /**
         * Instantiates BMP Atmospheric Pressure Sensor based on a given string.
         *
         * @param initStr string containing specific information for BMP Atmospheric Pressure Sensor.
         */
        BMPX8X(std::string initStr);

        /**
         * BMPX8X object destructor.
         */
        virtual ~BMPX8X();

        /**
         * Query the device and update the internal state.  This
         * method must be called before calling getPressure(),
         * getTemperature(), getSealevelPressure(), and getAltitude()
         * to retrieve values.
         *
         * @throws std::runtime_error on failure.
         */
        void update();

        /**
         * Reset the device to power-on defaults.  All calibration
         * data is lost when the device is reset, so you should call
         * init() before attempting to use the device.
         *
         * @throws std::runtime_error on failure.
         */
        void reset();

        /**
         * Initialize the device, read calibration data, and start
         * operation.  This function is called from the constructor,
         * so it will not typically need to be called by a user unless
         * the device is reset.
         *
         * @param oss One of the BMPX8X_OSS_T values.  The
         * default is BMPX8X_OSS_ULTRAHIGHRES.
         * @throws std::runtime_error on failure.
         */
        void init(BMPX8X_OSS_T oss=BMPX8X_OSS_ULTRAHIGHRES);

        /**
         * Set the oversampling (precision mode) of the device.
         * Higher precision requires more time to complete.  This call
         * takes effect the next time update() is called.
         *
         * @param oss One of the BMPX8X_OSS_T values.  The
         * default is BMPX8X_OSS_ULTRAHIGHRES.
         */
        void setOversampling(BMPX8X_OSS_T oss=BMPX8X_OSS_ULTRAHIGHRES);

        /**
         * Returns the calculated pressure in Pascals.  update() must
         * have been called prior to calling this function.
         *
         * @returns The pressure in Pascals.
         */
        virtual float getPressure();

        /**
         * Returns the calculated temperature in Celsius.  update()
         * must have been called prior to calling this function.
         *
         * @returns The temperature in Celsius.
         */
        virtual float getTemperature();

        /**
         * Using the supplied altitude in meters, compute the pressure
         * at sea level in Pascals.  update() must have been called
         * prior to calling this function.
         *
         * @param meters The altitude in meters.
         * @returns The computed sea level pressure in Pascals.
         */
        int getSealevelPressure(float meters);

        /**
         * Using the current calculated altitude, compute the pressure
         * at sea level in Pascals.  update() must have been called
         * prior to calling this function.
         *
         * @returns The computed sea level pressure in Pascals.
         */
        int getSealevelPressure()
        {
            return getSealevelPressure(getAltitude());
        }

        /**
         * Calculate the current altitude in meters, given a sea level
         * pressure in Pascals.  The default sea level pressure is
         * 101325 Pascals.  update() must have been called prior to
         * calling this function.
         *
         * @param sealevelPressure The pressure at sea level in
         * Pascals.  The default is 101325 Pascals.
         * @returns the computed altitude in meters.
         */
        float getAltitude(int sealevelPressure = 101325);

        /**
         * Return latest calculated temperature value in Celsius.  See
         * ITemperatureSensor.
         *
         * @return The current temperature in Celsius.
         */
        int getTemperatureCelsius()
        {
            update();
            return (int)getTemperature();
        }

        /**
         * Return latest calculated pressure value in Pascals.  See
         * IPressureSensor.
         *
         * @return The current pressure in Pascals.
         */
        int getPressurePa()
        {
            update();
            return getPressure();
        }

        /**
         * Returns the name of module.
         *
         * @return The name of the module.
         */
        const char *getModuleName()
        {
            return "BMPX8X";
        }

    protected:
        // our underlying C context.
        bmpx8x_context m_bmpx8x;
        mraa::MraaIo mraaIo;

        /**
         * Read a register.
         *
         * @param reg The register to read.
         * @return The value of the register.
         */
        uint8_t readReg(uint8_t reg);

        /**
         * Read contiguous registers into a buffer.
         *
         * @param buffer The buffer to store the results.
         * @param len The number of registers to read.
         * @return The number of bytes read.
         * @throws std::runtime_error on failure.
         */
        int readRegs(uint8_t reg, uint8_t *buffer, int len);

        /**
         * Write to a register.
         *
         * @param reg The register to write to.
         * @param val The value to write.
         * @throws std::runtime_error on failure.
         */
        void writeReg(uint8_t reg, uint8_t val);

    private:
        /* Disable implicit copy and assignment operators */
        BMPX8X(const BMPX8X&) = delete;
        BMPX8X &operator=(const BMPX8X&) = delete;
    };
}
