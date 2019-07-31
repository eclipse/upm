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

/**
 * Rotary angle sensor initialization function
 *
 * @param pin analog pin to use
 * @param aRef reference voltage
 * @return sensor context
 */
rotary_context rotary_init(int pin, float aRef);

/**
 * Rotary destructor
 *
 * @param dev sensor context pointer
 */
void rotary_close(rotary_context dev);

/**
 * Gets the voltage value from the analog pin
 *
 * @param dev sensor context pointer
 * @param volts sensor voltage value
 * @return result of the operation
 */
upm_result_t rotary_get_value_voltage(const rotary_context dev, float* volts);

/**
 * Gets the position of the rotary angle sensor in degrees
 *
 * @param dev sensor context pointer
 * @param rotval sensor position in degrees
 * @return result of the operation
 */
upm_result_t rotary_get_value_angle(rotary_context dev, float* rotval);

#endif /* ROTARY_H_ */
