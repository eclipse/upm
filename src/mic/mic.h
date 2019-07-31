/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * 	   Sisinty Sasmita Patra <sisinty.s.patra@intel.com>
 *
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
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
