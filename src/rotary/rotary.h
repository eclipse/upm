/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * 	   Sisinty Sasmita Patra <sisinty.s.patra@intel.com>
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

#ifndef ROTARY_H_
#define ROTARY_H_

#include <unistd.h>

#include "upm.h"
#include <mraa/aio.h>

/**
 * @file rotary.h
 * @library rotary
 * @brief C API for the Rotary Angle Sensor
 *
 * @include rotary.c
 */

/**
 * driver context
 */
typedef struct _rotary_context {
    mraa_aio_context aio;
    float m_aRef;
    int16_t m_aRes;
} *rotary_context;

#define ROTARY_MAX_ANGLE 300

rotary_context rotary_init(int pin, float aRef);

void rotary_close(rotary_context dev);

upm_result_t rotary_get_value_voltage(const rotary_context dev, float* volts);

// degrees only
upm_result_t rotary_get_value_angle(rotary_context dev, float* rotval);

#endif /* ROTARY_H_ */
