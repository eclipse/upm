/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * Based on original C++ driver written by:
 * Author: Zion Orent <sorent@ics.com>
 * Copyright (c) 2014 Intel Corporation.
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
