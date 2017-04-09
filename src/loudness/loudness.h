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

#ifndef LOUDNESS_LOUDNESS_H_
#define LOUDNESS_LOUDNESS_H_

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "upm.h"
#include "mraa/aio.h"

/**
 * @file loudness.h
 * @library loudness
 * @brief C API for the Analog Loudness Sensor
 *
 * @include loudness.c
 */

/**
 * device context
 */
typedef struct _loudness_context {
    mraa_aio_context            aio;
} *loudness_context;

/**
 * Loudness Initialization function
 *
 * @param pin AIO pin to use
 * @return void* pointer to the sensor struct
 */
loudness_context loudness_init(int pin);

/**
 * Loudness destructor
 *
 * @param void* pointer to the sensor struct
 */
void loudness_close(loudness_context dev);

/**
 * Function gets the loudness raw value (currently)
 *
 * @param void* pointer to the sensor struct
 * @param int* pointer to store the loudness value.
 * @return upm_result_t UPM success/error code
 */
upm_result_t loudness_get_value(loudness_context dev, int* val);

#endif /* LOUDNESS_LOUDNESS_H_ */
