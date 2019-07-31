/*
 * Author: Sisinty Sasmita Patra <sisinty.s.patra@intel.com>
 *
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
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

/**
 * Analog temperature sensor initialization function
 *
 * @param pin analog pin to use
 * @return sensor context
 */
temperature_context temperature_init(int pin);

/**
 * Analog temperature sensor destructor
 *
 * @param dev sensor context pointer
 */
void temperature_close(temperature_context dev);

/**
 * Gets the temperature in Celsius from the sensor
 *
 * @param dev sensor context pointer
 * @param tempval temperature value in Celsius
 * @return result of the operation
 */
upm_result_t temperature_get_value(temperature_context dev,
                                   float* tempval);

#endif /* TEMPERATURE_H_ */
