/*
 * Author: Noel Eck <noel.eck@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "upm.h"
#include "mraa/aio.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file gsr.h
 * @library gsr
 * @brief C API for the GSR Sensor
 *
 * @include gsr.c
 */

/**
 * driver context
 */
typedef struct _gsr_context {
    /* mraa aio pin context */
    mraa_aio_context aio;
    /* Analog voltage reference */
    float m_aRef;
    /* Scale */
    float m_scale;
    /* Offset in sensor units */
    float m_offset;
} *gsr_context;

/**
 * Initialize analog sensor
 * @param pin Analog pin
 * @return sensor context
 */
gsr_context gsr_init(int16_t pin);

/**
 * Analog sensor destructor
 * @param sensor context pointer
 */
void gsr_close(gsr_context dev);

/**
 * Set ADC reference voltage
 * @param dev sensor context pointer
 * @param aref ADC reference voltage
 * @return Function result code
 */
upm_result_t gsr_set_aref(const gsr_context dev, float aref);

/**
 * Set sensor scale.  This scale is applied to the return value:
 *     counts = counts * scale
 * @param dev sensor context pointer
 * @param scale count scale value used
 * @return Function result code
 */
upm_result_t gsr_set_scale(const gsr_context dev, float scale);

/**
 * Set sensor offset.  This offset is applied to the return value:
 *     value = value + offset
 * @param dev sensor context pointer
 * @param offset count offset value used
 * @return Function result code
 */
upm_result_t gsr_set_offset(const gsr_context dev, float offset);

/**
 * Get sensor aref
 * @param dev sensor context pointer
 * @return Sensor ADC reference voltage
 */
float gsr_get_aref(const gsr_context dev);

/**
 * Get sensor scale
 * @param dev sensor context pointer
 * @return Sensor scale
 */
float gsr_get_scale(const gsr_context dev);

/**
 * Get sensor offset
 * @param dev sensor context pointer
 * @return Sensor offset
 */
float gsr_get_offset(const gsr_context dev);

/**
 * Read normalized value for sensor
 * @param dev sensor context pointer
 * @param *value Normalized value (0.0 -> 1.0)
 * @return Function result code
 */
upm_result_t gsr_get_normalized(const gsr_context dev, float *value);

/**
 * Read raw voltage from the sensor
 * @param dev sensor context pointer
 * @param *value Raw sensor voltage
 * @return Function result code
 */
upm_result_t gsr_get_raw_volts(const gsr_context dev, float *value);

/**
 * Read scaled/offset voltage from the sensor
 * @param dev sensor context pointer
 * @param *value Adjusted sensor voltage
 * @return Function result code
 */
upm_result_t gsr_get_volts(const gsr_context dev, float *value);
#ifdef __cplusplus
}
#endif
