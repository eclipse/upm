/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
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
#include <vector>

#include <mraa/gpio.hpp>
#include "lsm303d.h"

#include <interfaces/iAcceleration.hpp>

namespace upm {

    /**
     * @brief LSM303D Ultra-compact eCompass Module
     * @defgroup lsm303d libupm-lsm303d
     * @ingroup i2c gpio stmicro compass accelerometer
     */

    /**
     * @library lsm303d
     * @sensor lsm303d
     * @comname Ultra-compact high-performance eCompass module
     * @type compass
     * @man stmicro
     * @con i2c gpio
     * @web http://www.st.com/en/mems-and-sensors/lsm303d.html
     *
     * @brief API for the LSM303D 3-Axis Geomagnetic Sensor
     *
     * The LSM303D is an ultra-low-power high-performance
     * system-in-package featuring a 3D digital linear acceleration
     * sensor and a 3D digital magnetic sensor.  The LSM303D has
     * linear acceleration full scales of 2g/4g/8g/16g and a
     * magnetic field dynamic range of 50 Gauss.
     *
     * Not all functionality of this chip has been implemented in this
     * driver, however all the pieces are present to add any desired
     * functionality.  This driver supports only I2C operation.
     *
     * This device requires 3.3v operation.
     *
     * @snippet lsm303d.cxx Interesting
     */

    class LSM303D: virtual public iAcceleration {
    public:
        /**
         * LSM303D constructor
         *
         * This driver only supports I2C.
         *
         * Due to the fact that this chip is currently obsolete, we only
         * support minimum functionality.
         *
         * @param bus I2C bus to use
         * @param addr The I2C address of the device
         * @return The device context, or NULL if an error occurred
         * @throws std::runtime_error on failure.
         */
        LSM303D(int bus=LSM303D_DEFAULT_I2C_BUS,
                int addr=LSM303D_DEFAULT_I2C_ADDR);

        /**
         * LSM303D Destructor
         */
        ~LSM303D();

        /**
         * Update the internal stored values from sensor data.  This
         * method must be called before querying the acceleration,
         * magnetometer, or temperature.
         *
         * @throws std::runtime_error on failure
         */
        void update();

        /**
         * Return magnetometer data in micro-Teslas (uT).  update() must
         * have been called prior to calling this method.
         *
         * @param x Pointer to a floating point value that will have the
         * current x component placed into it
         * @param y Pointer to a floating point value that will have the
         * current y component placed into it
         * @param z Pointer to a floating point value that will have the
         * current z component placed into it
         */
        void getMagnetometer(float *x, float *y, float *z);

        /**
         * Return magnetometer data in micro-Teslas (uT) in the form
         * of a floating point vector.  update() must have been called
         * prior to calling this method.
         *
         * @return A floating point vector containing x, y, and z in
         * that order
         */
        std::vector<float> getMagnetometer();

        /**
         * Return acceleration data in gravities.  update() must have
         * been called prior to calling this method.
         *
         * @param x Pointer to a floating point value that will have the
         * current x component placed into it
         * @param y Pointer to a floating point value that will have the
         * current y component placed into it
         * @param z Pointer to a floating point value that will have the
         * current z component placed into it
         */
        void getAccelerometer(float *x, float *y, float *z);

        /**
         * Return acceleration data in gravities in the form of a
         * floating point vector.  update() must have been called
         * prior to calling this method.
         *
         * @return A floating point vector containing x, y, and z in
         * that order
         */
        std::vector<float> getAccelerometer();
        
        /**
         * get acceleration values
         * 
         * @return stl vector of size 3 representing the 3 axis
         */
        virtual std::vector<float> getAcceleration();

        /**
         * Return temperature data in degrees Celsius.  NOTE: This is
         * not the ambient room temperature.  update() must have been
         * called prior to calling this method.
         *
         * @return Temperature in degrees Celsius
         */
        float getTemperature();

        /**
         * Initialize the device and start operation.  This function
         * is called from the constructor so it will not typically
         * need to be called by a user unless the device is reset.  It
         * will initialize the accelerometer and magnetometer (if
         * enabled) to certain default running modes.
         *
         * @param res One of the LSM303D_M_RES_T values.  This value
         * sets the resolution of the magnetometer.  At init time,
         * this value is set to LSM303D_M_RES_HIGH.
         * @throws std::runtime_error on failure
         */
        void init(LSM303D_M_RES_T res);

        /**
         * Set the full scale (sensitivity) value for the
         * accelerometer.  This device supports 2G, 4G, 6G, 8G, and
         * 16G full scale modes.
         *
         * @param fs One of the LSM303D_AFS_T values
         * @throws std::runtime_error on failure
         */
        void setAccelerometerFullScale(LSM303D_AFS_T fs);

        /**
         * Set the full scale (sensitivity) value for the
         * magnetometer.  This device supports 2, 4, 8, and
         * 16 Gauss full scale modes.
         *
         * @param fs One of the LSM303D_MFS_T values
         * @throws std::runtime_error on failure
         */
        void setMagnetometerFullScale(LSM303D_MFS_T fs);

        /**
         * Set the accelerometer output data rate (ODR)
         *
         * @param odr One of the LSM303D_AODR_T values.  The default
         * set at initialization time is LSM303D_AODR_100HZ.
         * @throws std::runtime_error on failure
         */
        void setAccelerometerODR(LSM303D_AODR_T odr);

        /**
         * Set the magnetometer output data rate (ODR)
         *
         * @param odr One of the LSM303D_MODR_T values.
         * The default set at initialization time is
         * LSM303D_MODR_12_5HZ.
         * @throws std::runtime_error on failure
         */
        void setMagnetometerODR(LSM303D_MODR_T odr);

    protected:
        // our underlying device context
        lsm303d_context m_lsm303d;

        /**
         * Read a register
         *
         * @param reg The register to read
         * @return The value of the register
         */
        uint8_t readReg(uint8_t reg);

        /**
         * Read contiguous registers into a buffer
         *
         * @param buffer The buffer to store the results
         * @param len The number of registers to read
         * @return The number of bytes read
         * @throws std::runtime_error on failure
         */
        int readRegs(uint8_t reg, uint8_t *buffer, int len);

        /**
         * Write to a register
         *
         * @param reg The register to write to
         * @param val The value to write
         * @throws std::runtime_error on failure
         */
        void writeReg(uint8_t reg, uint8_t val);

    private:
        /* Disable implicit copy and assignment operators */
        LSM303D(const LSM303D&) = delete;
        LSM303D &operator=(const LSM303D&) = delete;
    };
}
