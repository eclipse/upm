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

#include <unistd.h>
#include <string.h>

#include <mraa/i2c.h>
#include <mraa/gpio.h>

#include "upm.h"

#include "lsm303d_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @file lsm303d.h
     * @library lsm303d
     * @brief C API for the lsm303d driver
     *
     * @include lsm303d.c
     */

    /**
     * Device context
     */
    typedef struct _lsm303d_context {
        mraa_i2c_context i2c;

        // uncompensated temperature in C
        float temperature;

        // accelerometer scaling
        float accScale;

        // magnetometer scaling
        float magScale;

        // uncompensated acc data
        float accX;
        float accY;
        float accZ;

        // uncompensated mag data
        float magX;
        float magY;
        float magZ;
    } *lsm303d_context;


    /**
     * LSM303D initialization
     *
     * This driver only supports I2C.
     *
     * Due to the fact that this chip is currently obsolete, we only
     * support minimum functionality.
     *
     * @param bus I2C bus to use
     * @param addr The I2C address of the device
     * @return The device context, or NULL if an error occurred
     */
    lsm303d_context lsm303d_init(int bus, int addr);

    /**
     * LSM303D Destructor
     *
     * @param dev The device context
     */
    void lsm303d_close(lsm303d_context dev);

    /**
     * Update the internal stored values from sensor data
     *
     * @param dev The device context
     * @return UPM result
     */
    upm_result_t lsm303d_update(const lsm303d_context dev);

    /**
     * Return magnetometer data in micro-Teslas (uT).  update() must
     * have been called prior to calling this function.
     *
     * @param dev The device context
     * @param x Pointer to a floating point value that will have the
     * current x component placed into it
     * @param y Pointer to a floating point value that will have the
     * current y component placed into it
     * @param z Pointer to a floating point value that will have the
     * current z component placed into it
     */
    void lsm303d_get_magnetometer(const lsm303d_context dev,
                                  float *x, float *y, float *z);

    /**
     * Return accelerometer data in gravities.  update() must have
     * been called prior to calling this function.
     *
     * @param dev The device context
     * @param x Pointer to a floating point value that will have the
     * current x component placed into it
     * @param y Pointer to a floating point value that will have the
     * current y component placed into it
     * @param z Pointer to a floating point value that will have the
     * current z component placed into it
     */
    void lsm303d_get_accelerometer(const lsm303d_context dev,
                                   float *x, float *y, float *z);

    /**
     * Return temperature data in degrees Celsius..  update() must
     * have been called prior to calling this function.
     *
     * @param dev The device context
     * @return Temperature in degrees Celsius
     */
    float lsm303d_get_temperature(const lsm303d_context dev);

    /**
     * Set the full scale (sensitivity) value for the accelerometer.
     * This device supports 2G, 4G, 6G, 8G, and 16G full scale modes.
     *
     * @param dev The device context
     * @param fs One of the LSM303D_AFS_T values
     * @return UPM result
     */
    upm_result_t lsm303d_set_acc_full_scale(const lsm303d_context dev,
                                            LSM303D_AFS_T fs);

    /**
     * Set the full scale (sensitivity) value for the magnetometer.
     * This device supports 2, 4, 8, and 16 Gauss.
     *
     * @param dev The device context
     * @param fs One of the LSM303D_MFS_T values
     * @return UPM result
     */
    upm_result_t lsm303d_set_mag_full_scale(const lsm303d_context dev,
                                            LSM303D_MFS_T fs);

    /**
     * Initialize the device and start operation.  This function is
     * called from the constructor so it will not typically need to be
     * called by a user unless the device is reset.  It will
     * initialize the accelerometer and magnetometer (if enabled) to
     * certain default running modes.
     *
     * @param dev The device context
     * @param res One of the LSM303D_M_RES_T values.  This value sets
     * the resolution of the magnetometer.  At init time, this value
     * is set to LSM303D_M_RES_HIGH.
     * @return UPM result
     */
    upm_result_t lsm303d_devinit(const lsm303d_context dev,
                                 LSM303D_M_RES_T res);

    /**
     * Set the accelerometer (acc) output data rate (odr)
     *
     * @param dev The device context
     * @param odr One of the LSM303D_AODR_T values.  The default
     * set at initialization time is LSM303D_AODR_100HZ.
     * @return UPM result
     */
    upm_result_t lsm303d_set_acc_odr(const lsm303d_context dev,
                                     LSM303D_AODR_T odr);

    /**
     * Set the magnetometer (mag) output data rate (odr)
     *
     * @param dev The device context
     * @param odr One of the LSM303D_MODR_T values.  The default
     * set at initialization time is LSM303D_MODR_12_5HZ.
     * @return UPM result
     */
    upm_result_t lsm303d_set_mag_odr(const lsm303d_context dev,
                                     LSM303D_MODR_T odr);

    /**
     * Read a register.
     *
     * @param dev The device context
     * @param reg The register to read
     * @return The value of the register
     */
    uint8_t lsm303d_read_reg(const lsm303d_context dev, uint8_t reg);

    /**
     * Read contiguous registers into a buffer
     *
     * @param dev The device context
     * @param buffer The buffer to store the results
     * @param len The number of registers to read
     * @return The number of bytes read, or -1 on error
     */
    int lsm303d_read_regs(const lsm303d_context dev, uint8_t reg,
                          uint8_t *buffer, int len);

    /**
     * Write to a register
     *
     * @param dev The device context
     * @param reg The register to write to
     * @param val The value to write
     * @return UPM result
     */
    upm_result_t lsm303d_write_reg(const lsm303d_context dev,
                                   uint8_t reg, uint8_t val);

#ifdef __cplusplus
}
#endif
