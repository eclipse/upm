/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
