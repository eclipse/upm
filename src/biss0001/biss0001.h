/*
 * Author: Jon Trulson <jtrulson@ics.com>
 *         Zion Orent <zorent@ics.com>
 * Copyright (c) 2016 Intel Corporation.
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
#include <mraa/gpio.h>

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @file biss0001.h
     * @library biss0001
     * @brief C API for the BISS0001 Motion Sensor
     *
     * @include biss0001.c
     */

    /**
     * Device context
     */
    typedef struct _biss0001_context {
        mraa_gpio_context        gpio;
    } *biss0001_context;

    /**
     * BISS0001 initilaizer
     *
     * @param pin Digital pin to use.
     * @return an initialized device context on success, NULL on error.
     */
    biss0001_context biss0001_init(unsigned int pin);

    /**
     * BISS0001 close function
     *
     * @param dev The device context.
     */
    void biss0001_close(biss0001_context dev);

    /**
     * Gets the motion value from the sensor
     *
     * @param dev The device context.
     * @return true if motion was detected, false otherwise
     */
    bool biss0001_motion_detected(const biss0001_context dev);

#ifdef __cplusplus
}
#endif
