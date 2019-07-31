/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * Ported based on original C++ code by:
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * Contributions: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 * Copyright (c) 2014 Intel Corporation.
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
#include <unistd.h>

#include <upm.h>
#include <mraa/i2c.h>

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @file hmc5883l.h
     * @library hmc5883l
     * @brief HMC5883L 3-Axis Digital Compass
     *
     */

    /**
     * Device context
     */
    typedef struct _hmc5883l_context {
        mraa_i2c_context i2c;

        int16_t coords[3];
        float   declination;
    } *hmc5883l_context;


    /**
     * Initialize an Hmc5883l device
     *
     * @param bus Number of the used I2C bus
     * @return Device context, or NULL on error
     */
    hmc5883l_context hmc5883l_init(int bus);

    /**
     * Close the device
     *
     * @param Device context
     */
    void hmc5883l_close(hmc5883l_context dev);

    /**
     * Updates the values by reading from I2C
     *
     * @param Device context
     * @return UPM result
     */
    upm_result_t hmc5883l_update(const hmc5883l_context dev);

    /*
     * Returns the direction.  hmc5883l_update() must have been called
     * prior to calling this function.
     *
     * @param Device context
     * @return Direction
     */
    float hmc5883l_direction(const hmc5883l_context dev);

    /*
     * Returns the heading.  hmc5883l_update() must have been called
     * prior to calling this function.
     *
     * @param Device context
     * @return Heading
     */
    float hmc5883l_heading(const hmc5883l_context dev);

    /**
     * Returns a pointer to an int[3] that contains the coordinates as
     * integers.  hmc5883l_update() must have been called prior to calling
     * this function.
     *
     * @param Device context
     * @return *int to an int[3] (X, Y, Z)
     */
    const int16_t *hmc5883l_coordinates(const hmc5883l_context dev);

    /**
     * Sets the magnetic declination for better accuracy.
     * hmc5883l_update() must have been called prior to calling this
     * function.
     *
     * @param Device context
     * @param dec The magnetic declination
     */
    void hmc5883l_set_declination(const hmc5883l_context dev, float dec);

    /**
     * Gets the current magnetic declination value.  hmc5883l_update()
     * must have been called prior to calling this function.
     *
     * @param Device context
     * @return Magnetic declination as a floating-point value
     */
    float hmc5883l_get_declination(const hmc5883l_context dev);

#ifdef __cplusplus
}
#endif
