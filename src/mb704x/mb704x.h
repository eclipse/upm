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
#include <upm.h>
#include <mraa/i2c.h>
#include <mraa/gpio.h>

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @file mb704x.h
     * @library mb704x
     * @brief C API for the MB704x MaxSonar-WR Ultrasonic Ranger
     *
     * @include mb704x.c
     */

    /**
     * Device context
     */
    typedef struct _mb704x_context {
        mraa_i2c_context         i2c;
    } *mb704x_context;

    /**
     * MB704X Initializer
     *
     * @param bus Specify which the I2C bus to use.
     * @param addr Specify the I2C address to use.  The default is 112.
     * @return an initialized device context on success, NULL on error.
     */
    mb704x_context mb704x_init(unsigned int bus, int addr);

    /**
     * MB704X sensor close function
     */
    void mb704x_close(mb704x_context dev);

    /**
     * Query the device for a range reading.  The range will be
     * reported in centimeters (cm).
     *
     * @param dev Device context
     * @return Measured range, -1 on error. The range is reported in
     * centimeters (cm).
     */
    int mb704x_get_range(const mb704x_context dev);


#ifdef __cplusplus
}
#endif
