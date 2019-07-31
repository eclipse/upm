/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * Based on original C++ driver written by:
 * Author: Zion Orent <sorent@ics.com>
 * Copyright (c) 2014 Intel Corporation.
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
#include "ppd42ns_data.h"

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @file ppd42ns.h
     * @library ppd42ns
     * @brief C API for the ppd42ns driver
     *
     * @include ppd42ns.c
     */

    /**
     * Device context
     */
    typedef struct _ppd42ns_context {
        mraa_gpio_context gpio;

    } *ppd42ns_context;

    /**
     * PPD42NS initialization
     *
     * @param pin Digital pin to use
     * @return ppd42ns device context
     */
    ppd42ns_context ppd42ns_init(int pin);

    /**
     * PPD42NS close
     *
     * @param dev Device context.
     */
    void ppd42ns_close(ppd42ns_context dev);

    /**
     * Prints dust concentration
     *
     * @param dev Device context.
     * @return ppd42ns_dust_data Contains data from the dust sensor
     */
    ppd42ns_dust_data ppd42ns_get_data(const ppd42ns_context dev);

#ifdef __cplusplus
}
#endif
