/*
 * Author: Sisinty Sasmita Patra <sisinty.s.patra@intel.com>
 *
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

#ifndef TEMPERATURE_H_
#define TEMPERATURE_H_

#include <stdlib.h>
#include <unistd.h>

#include "upm.h"
#include "mraa/aio.h"

/**
 * @file temperature.h
 * @library temperature
 * @brief C API for Analog Temperature (NTC) Sensors
 *
 * @include temperature.c
 */

/**
 * driver context
 */
typedef struct _temperature_context {
    mraa_aio_context aio;
    int16_t m_aRes;
} *temperature_context;


temperature_context temperature_init(int pin);

void temperature_close(temperature_context dev);

// Celsius
upm_result_t temperature_get_value(temperature_context dev,
                                   float* tempval);

#endif /* TEMPERATURE_H_ */
