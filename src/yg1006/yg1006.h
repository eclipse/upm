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

#include <stdlib.h>
#include <stdio.h>
#include <upm.h>
#include <mraa/gpio.h>

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @file yg1006.h
     * @library yg1006
     * @brief C API for the YG1006 Flame Sensor
     *
     * @include yg1006.c
     */

    /**
     * Device context
     */
    typedef struct _yg1006_context {
        mraa_gpio_context        gpio;
    } *yg1006_context;

    /**
     * YG1006 initializer
     *
     * @param pin Digital pin to use
     * @return an initialized device context on success, NULL on error.
     */
    yg1006_context yg1006_init(unsigned int pin);

    /**
     * YG1006 close function
     */
    void yg1006_close(yg1006_context dev);

    /**
     * Determines whether a flame has been detected
     *
     * @return true if a flame or another comparable light source has
     * been detected
     */
    bool yg1006_flame_detected(const yg1006_context dev);

#ifdef __cplusplus
}
#endif
