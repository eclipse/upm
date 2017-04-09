/*
 * Author: Jon Trulson <jtrulson@ics.com>
 *         Abhishek Malik <abhishek.malik@intel.com>
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

#ifndef AD8232_H_
#define AD8232_H_

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "upm.h"
#include "mraa/aio.h"
#include "mraa/gpio.h"

#define AD8232_DEFAULT_AREF  3.3

/**
 * @file ad8232.h
 * @library ad8232
 * @brief C API for the AD8232 Heart Rate Monitor
 *
 * @include ad8232.c
 */

/**
 * device context
 */
typedef struct _ad8232_context {
    mraa_aio_context        aio;
    mraa_gpio_context       gpio_lo_plus;
    mraa_gpio_context       gpio_lo_minus;
    uint8_t                 aio_pin;
    uint8_t                 gpio_lo_plus_pin;
    uint8_t                 gpio_lo_minus_pin;
    int                     a_res;
} *ad8232_context;

/**
 * Sensor Init function
 */
ad8232_context ad8232_init(int lo_plus, int lo_minus, int output, float a_ref);

void ad8232_close(ad8232_context dev);

upm_result_t ad8232_get_value(ad8232_context dev, int* value);

#endif /* AD8232_H_ */