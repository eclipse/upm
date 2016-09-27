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
