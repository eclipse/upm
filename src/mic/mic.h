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

#ifndef MIC_H_
#define MIC_H_

#pragma once
#include <stdlib.h>
#include <unistd.h>

#include "upm.h"
#include "mraa/aio.h"

/**
 * @file mic.h
 * @library mic
 * @brief C API for the Analog Microphone
 *
 * @include mic.c
 */

/**
 * device context
 */
typedef struct _mic_context {
    mraa_aio_context aio;
    uint16_t analog_pin;
} *mic_context;

/**
 * Microphone sensor initialization function
 *
 * @param pin analog pin to use
 * @return sensor context
 */
mic_context mic_init(int pin);

/**
 * Microphone sensor destructor
 *
 * @param dev sensor context pointer
 */
void mic_close(mic_context dev);

/**
 * Gets a sample from the microphone
 *
 * @param dev sensor context pointer
 * @param micval microphone value in ADC counts
 * @return result of the operation
 */
upm_result_t mic_get_value(mic_context dev, float* micval);

#endif /* MIC_H_ */
