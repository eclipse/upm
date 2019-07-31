/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <upm.h>

#include <mraa/i2c.h>

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @file nunchuck.h
     * @library nunchuck
     * @brief C API for the nunchuck driver
     *
     * @include nunchuck.c
     */

    /**
     * Device context
     */
    typedef struct _nunchuck_context {
        mraa_i2c_context  i2c;

        // sticks - x, y
        int stickX;
        int stickY;

        // accel
        int accelX;
        int accelY;
        int accelZ;

        // buttons
        bool buttonC;
        bool buttonZ;

    } *nunchuck_context;

    /**
     * NUNCHUCK constructor
     *
     * @param bus I2C bus to use
     * @return A Device context, or NULL on error
     */
    nunchuck_context nunchuck_init(int bus);

    /**
     * Close the device and deallocate all resources.
     *
     * @param dev Device context
     */
    void nunchuck_close(nunchuck_context dev);

    /**
     * Reads and updates the current state of the controller.
     *
     * @param dev Device context
     * @return UPM result
     */
    upm_result_t nunchuck_update(const nunchuck_context dev);

    /**
     * Returns the current analog stick X and Y positions.
     * nunchuck_update() must have been called prior to calling this
     * function.
     *
     * @param dev Device context
     * @param x Pointer in which the X value will be stored
     * @param y Pointer in which the Y value will be stored
     */
    void nunchuck_get_stick(const nunchuck_context dev, int *x, int *y);

    /**
     * Returns the current accelerometer values.  nunchuck_update()
     * must have been called prior to calling this function.
     *
     * @param dev Device context
     * @param x Pointer in which the X value will be stored
     * @param y Pointer in which the Y value will be stored
     * @param z Pointer in which the Z value will be stored
     */
    void nunchuck_get_acceleration(const nunchuck_context dev,
                                   int *x, int *y, int *z);

    /**
     * Returns the current button values values.  nunchuck_update()
     * must have been called prior to calling this function.
     *
     * @param dev Device context
     * @param c Pointer in which the C button value will be stored
     * @param z Pointer in which the Z button value will be stored
     */
    void nunchuck_get_buttons(const nunchuck_context dev, bool *c, bool *z);

#ifdef __cplusplus
}
#endif
