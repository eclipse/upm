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

#pragma once

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "upm.h"
#include "mraa/aio.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file moisture.h
 * @library moisture
 * @brief C API for the Analog Moisture Sensor
 *
 * @include moisture.c
 */

/**
 * device context
 */
typedef struct _moisture_context {
    mraa_aio_context    aio;
    uint16_t            analog_pin;
} *moisture_context;

/**
 * Init function
 *
 * @param pin analog pin number
 * @return void* pointer to the sensor struct
 */
moisture_context moisture_init(int pin);

/**
 * Close function
 *
 * @param dev pointer to the sensor structure
 */
void moisture_close(moisture_context dev);

/**
 * Get Moisture function.
 *
 * @param dev pointer to the sensor struct
 * @param moisture pointer that will be used to store the
 * output value from the sensor
 */
upm_result_t moisture_get_moisture(moisture_context dev,
                                        int* moisture);

#ifdef __cplusplus
}
#endif
