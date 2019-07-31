/*
 * Author: Jon Trulson <jtrulson@ics.com>
 *         Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
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
