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

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <upm.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "bosch_bmi160.h"
#include "bmi160_defs.h"

  /**
   * @brief BMI160 3-axis Accelerometer, Gyroscope and (optionally) a
   * Magnetometer
   *
   * The Bosch BMI160 is a 3-axis Accelerometer and Gyroscope.
   * Additionally it supports an external Magnetometer, accessed
   * through the BMI160's register interface.  This driver was
   * developed with a BMI160 "Shuttle" board, which included a BMM150
   * Magnetometer.
   *
   * The device is driven by either 1.8v or 3.3vdc.  This driver
   * incorporates the Bosch BMI160 driver code at
   * https://github.com/BoschSensortec/BMI160_driver
   *
   * The Bosch driver code does not provide a mechanism for passing
   * user data around (like the device context).  For this reason,
   * only one instance of this driver can be used in a given process,
   * due to the use of static data in the driver.
   *
   * While not all of the functionality of this device is supported
   * initially, the inclusion of the Bosch driver in the source code
   * makes it possible to support whatever features are required that
   * the driver can support.
   *
   * @snippet bmi160.c Interesting
   */

    typedef struct _bmi160_context {
        // uncompensated accelerometer and gyroscope values
        float accelX;
        float accelY;
        float accelZ;

        float gyroX;
        float gyroY;
        float gyroZ;

        float magX;
        float magY;
        float magZ;

        unsigned int sensorTime;

        // accelerometer and gyro scaling factors, depending on their Full
        // Scale (Range) settings.
        float accelScale;
        float gyroScale;

        // is the magnetometer enabled?
        bool magEnabled;

    } *bmi160_context;

    /**
     * bmi160 constructor
     *
     * @param bus i2c/spi bus to use
     * @param address The address for this device if using I2C.  If
     * using SPI, supply -1 for this parameter.
     * @param cs_pin The GPIO to use for Chip Select (CS).  This is
     * only needed for SPI, and only if your SPI implementation
     * requires it.  Otherwise, just pass -1 if not using SPI, or your
     * CS is handled automatically by your SPI implementation.
     * @param enable_mag True, if you want to enable the magnetometer
     * @return an initialized device context on success, NULL on error.
     */
    bmi160_context bmi160_init(unsigned int bus, int address, int cs_pin,
                               bool enable_mag);

    /**
     * BMI160 Destructor
     *
     * @param dev Device context.
     */
    void bmi160_close(bmi160_context dev);

    /**
     * Take a measurement and store the current sensor values
     * internally.  This function must be called prior to retrieving
     * any sensor values, for example getAccelerometer().
     *
     * @param dev Device context.
     * @param dev sensor context
     */
    void bmi160_update(const bmi160_context dev);

    /**
     * set the scaling mode of the accelerometer
     *
     * @param dev Device context.
     * @param scale one of the ACCEL_RANGE_T values
     */
    void bmi160_set_accelerometer_scale(const bmi160_context dev,
                                        BMI160_ACC_RANGE_T scale);

    /**
     * set the scaling mode of the gyroscope
     *
     * @param dev Device context.
     * @param scale one of the GYRO_RANGE_T values
     */
    void bmi160_set_gyroscope_scale(const bmi160_context dev,
                                    BMI160_GYRO_RANGE_T scale);

    /**
     * Get the Accelerometer values.  The values returned are in
     * gravities.  update() must have been called prior to calling
     * this method.
     *
     * @param dev Device context.
     * @param x A pointer into which the X value will be returned
     * @param y A pointer into which the Y value will be returned
     * @param z A pointer into which the Z value will be returned
     */
    void bmi160_get_accelerometer(const bmi160_context dev,
                                  float *x, float *y, float *z);

    /**
     * Get the Gyroscope values.  The values returned are in degrees
     * per second.  update() must have been called prior to calling
     * this method.
     *
     * @param dev Device context.
     * @param x A pointer into which the X value will be returned
     * @param y A pointer into which the Y value will be returned
     * @param z A pointer into which the Z value will be returned
     */
    void bmi160_get_gyroscope(const bmi160_context dev,
                              float *x, float *y, float *z);

    /**
     * Get the Magnetometer values.  The values returned are in micro
     * Teslas.  update() must have been called prior to calling this
     * method.
     *
     * @param dev Device context.
     * @param x A pointer into which the X value will be returned
     * @param y A pointer into which the Y value will be returned
     * @param z A pointer into which the Z value will be returned
     */
    void bmi160_get_magnetometer(const bmi160_context dev,
                                 float *x, float *y, float *z);

    /**
     * Enable or disable the Magnetometer.  By default, the
     * magnetometer is enabled.
     *
     * @param dev Device context.
     * @param enable true to enable the magnetometer, false to disable.
     */
    void bmi160_enable_magnetometer(const bmi160_context dev, bool enable);

    /**
     * Return the sensor time.  This is a 24bit value that increments
     * every 39us.  It will wrap around once the 24b resolution is
     * exceeded.
     *
     * @param dev Device context.
     * @return The current sensor time.
     */
    unsigned int bmi160_get_time(const bmi160_context dev);

    /**
     * Perform a bus read.  This function is bus agnostic, and is used
     * by the bosch code to perform bus reads.  It is exposed here for
     * those users wishing to perform their own low level accesses.
     * This is a low level function, and should not be used unless you
     * know what you are doing.
     *
     * @param dev_addr For I2C operation, this is the I2C address.
     * For SPI, this argument is ignored.
     * @param reg_addr The register address to access.
     * @param reg_data A pointer to a buffer in which data will be read into.
     * @param cnt The number of bytes to read.
     * @return A return of 0 indicates no errors, non-zero indicates an error.
     */
    s8 bmi160_bus_read(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt);

    /**
     * Perform a bus write.  This function is bus agnostic, and is used
     * by the bosch code to perform bus writes.  It is exposed here for
     * those users wishing to perform their own low level accesses.
     * This is a low level function, and should not be used unless you
     * know what you are doing.
     *
     * @param dev_addr For I2C operation, this is the I2C address.
     * For SPI, this argument is ignored.
     * @param reg_addr The register address to access.
     * @param reg_data A pointer to a buffer containing data to write.
     * @param cnt The number of bytes to write.
     * @return A return of 0 indicates no errors, non-zero indicates an error.
     */
    s8 bmi160_bus_write(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt);

#ifdef __cplusplus
}
#endif
