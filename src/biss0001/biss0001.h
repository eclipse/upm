/*
 * Author: Jon Trulson <jtrulson@ics.com>
 *         Zion Orent <zorent@ics.com>
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
