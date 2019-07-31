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
     * Device context
     */
    typedef struct _linefinder_context {
        mraa_gpio_context gpio;
    } *linefinder_context;

    /**
     *  Line Finder inititialization
     *
     * @param pin Digital pin to use
     * @return device context
     */
    linefinder_context linefinder_init(int pin);

    /**
     * LineFinder close
     *
     * @param Device context
     */
    void linefinder_close(linefinder_context dev);

    /**
     * Determines whether white has been detected
     *
     * @param Device context
     * @return True if white is detected
     */
    bool linefinder_white_detected(const linefinder_context dev);

    /**
     * Determines whether black has been detected
     *
     * @param Device context
     * @return True if black is detected
     */
    bool linefinder_black_detected(const linefinder_context dev);


#ifdef __cplusplus
}
#endif
