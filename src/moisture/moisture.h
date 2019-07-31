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
