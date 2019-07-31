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

#include <unistd.h>
#include <string.h>

#include <mraa/i2c.h>

#include "upm.h"

#include "bmpx8x_defs.h"

#ifdef __cplusplus
extern "C" {
#endif


    /**
     * @file bmpx8x.h
     * @library bmpx8x
     * @brief C API for the bmpx8x driver
     *
     * @include bmpx8x.c
     */

    /**
     * Device context
     */
    typedef struct _bmpx8x_context {
        mraa_i2c_context i2c;

        // our oversampling (precision)
        BMPX8X_OSS_T oversampling;

        // compensated temperature and pressure
        float temperature;
        int pressure;

        // compensation coefficients
        int16_t ac1;
        int16_t ac2;
        int16_t ac3;
        uint16_t ac4;
        uint16_t ac5;
        uint16_t ac6;
        int16_t b1;
        int16_t b2;
        int16_t mb;
        int16_t mc;
        int16_t md;
    } *bmpx8x_context;


    /**
     * BMPX8X initialization.
     *
     * By default, the device is initialized to it's highest accuracy
     * (BMP085_OSS_ULTRAHIGHRES).
     *
     * @param bus I2C bus number.
     * @param addr I2C address of the device.
     * @return Device context, or NULL if an error occurred.
     */
    bmpx8x_context bmpx8x_init(int bus, int addr);

    /**
     * BMPX8X close function.
     *
     * @param dev Device context.
     */
    void bmpx8x_close(bmpx8x_context dev);

    /**
     * Return the chip ID.
     *
     * @param dev The device context.
     * @return The chip ID.
     */
    uint8_t bmpx8x_get_chip_id(const bmpx8x_context dev);

    /**
     * Initialize the device, read calibration data, and start
     * operation.  This function is called from bmpx8x_init() so it
     * will not typically need to be called by a user unless the
     * device is reset.  This method will call
     * bmpx8x_set_oversampling() with the passed parameter.
     *
     * @param dev The device context.
     * @param oss One of the BMPX8X_OSS_T values.  The default set
     * at bmpx8x_init() time is BMP085_OSS_ULTRAHIGHRES.
     * @return UPM result.
     */
    upm_result_t bmpx8x_devinit(const bmpx8x_context dev,
                                BMPX8X_OSS_T oss);

    /**
     * Perform a device reset.  The device will be reset as if it was
     * just powered on.  All compensation values will be lost.  You
     * should call bmpx8x_devinit() afterward, or perform the same
     * steps that bmpx8x_devinit() performs before attempting to use
     * the device.
     *
     * @param dev The device context.
     * @return UPM result.
     */
    upm_result_t bmpx8x_reset(const bmpx8x_context dev);

    /**
     * Update the internal stored values from sensor data.
     *
     * @param dev The device context.
     * @return UPM result.
     */
    upm_result_t bmpx8x_update(const bmpx8x_context dev);

    /**
     * Set the oversampling (precision mode) of the device.  Higher
     * precision requires more time to complete.
     *
     * @param dev The device context.
     * @param oss The desired oversampling mode, one of the
     * BMPX8X_OSS_T values.
     */
    void bmpx8x_set_oversampling(const bmpx8x_context dev,
                                 BMPX8X_OSS_T oss);

    /**
     * Returns the pressure in Pascals.  bmpx8x_update() must have
     * been called prior to calling this function.
     *
     * @param dev Device context.
     * @return The pressure in Pascals.
     */
    int bmpx8x_get_pressure(const bmpx8x_context dev);

    /**
     * Returns the temperature in degrees Celsius.  bmpx8x_update()
     * must have been called prior to calling this function.
     *
     * @param dev Device context.
     * @return The temperature in degrees Celsius.
     */
    float bmpx8x_get_temperature(const bmpx8x_context dev);

    /**
     * With a given current altitude, calculate pressure at sea level.
     * bmpx8x_update() must have been called prior to calling this
     * function.
     *
     * @param dev Device context.
     * @param altitude Current altitude in Meters.
     * @return The pressure in Pascals at sea level.
     */
    int bmpx8x_get_sealevel_pressure(const bmpx8x_context dev,
                                     float altitude);

    /**
     * With a given sea level, calculate altitude in meters.
     * bmpx8x_update() must have been called prior to calling this
     * function.
     *
     * @param dev Device context.
     * @param sealevel Sea level pressure in Pascals. If a negative
     * number, or zero is supplied, a default sealevel of 101325 Pa
     * will be used instead.
     * @return The current altitude in Meters.
     */
    float bmpx8x_get_altitude(const bmpx8x_context dev, int sealevel);

    /**
     * Read a register.
     *
     * @param dev The device context.
     * @param reg The register to read.
     * @return The value of the register.
     */
    uint8_t bmpx8x_read_reg(const bmpx8x_context dev, uint8_t reg);

    /**
     * Read contiguous registers into a buffer.
     *
     * @param dev The device context.
     * @param buffer The buffer to store the results.
     * @param len The number of registers to read.
     * @return The number of bytes read, or -1 on error.
     */
    int bmpx8x_read_regs(const bmpx8x_context dev, uint8_t reg,
                         uint8_t *buffer, int len);

    /**
     * Write to a register.
     *
     * @param dev The device context.
     * @param reg The register to write to.
     * @param val The value to write.
     * @return UPM result.
     */
    upm_result_t bmpx8x_write_reg(const bmpx8x_context dev,
                                  uint8_t reg, uint8_t val);

#ifdef __cplusplus
}
#endif
