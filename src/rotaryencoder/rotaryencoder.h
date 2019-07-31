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
     * @file rotaryencoder.h
     * @library rotaryencoder
     * @brief C API for the rotaryencoder driver
     *
     * @include rotaryencoder.c
     */

    /**
     * Device context
     */
    typedef struct _rotaryencoder_context {
        mraa_gpio_context gpioA;
        mraa_gpio_context gpioB;

        volatile int position;
    } *rotaryencoder_context;

    /**
     * RotaryEncoder initialization
     *
     * @param pinA Digital pin to use for signal A
     * @param pinB Digital pin to use for signal B
     */
    rotaryencoder_context rotaryencoder_init(int pin_a, int pin_b);

    /**
     * RotaryEncoder close function
     */
    void rotaryencoder_close(rotaryencoder_context dev);

    /**
     * Resets the position to a given number.
     *
     * @param count Integer to initialize the position to
     */
    void rotaryencoder_set_position(const rotaryencoder_context dev,
                                    int count);

    /**
     * Gets the position value.
     *
     */
    int rotaryencoder_get_position(const rotaryencoder_context dev);


#ifdef __cplusplus
}
#endif
