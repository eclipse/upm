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
 * @file light.h
 * @library light
 * @brief C API for Analog Light (Photocell) Sensors
 *
 * @include light.c
 */

/**
 * driver context
 */
typedef struct _light_context {
    /* mraa aio pin context */
    mraa_aio_context aio;
    /* Analog voltage reference */
    float m_aRef;
    /* Scale */
    float m_scale;
    /* Offset in sensor units */
    float m_offset;
} *light_context;

/**
 * Initialize analog sensor
 * @param pin Analog pin
 * @return sensor context
 */
light_context light_init(int16_t pin);

/**
 * Analog sensor destructor
 * @param sensor context pointer
 */
void light_close(light_context dev);

/**
 * Set ADC reference voltage
 * @param dev sensor context pointer
 * @param aref ADC reference voltage
 * @return Function result code
 */
upm_result_t light_set_aref(const light_context dev, float aref);

/**
 * Set sensor scale.  This scale is applied to the return value:
 *     counts = counts * scale
 * @param dev sensor context pointer
 * @param scale count scale value used
 * @return Function result code
 */
upm_result_t light_set_scale(const light_context dev, float scale);

/**
 * Set sensor offset.  This offset is applied to the return value:
 *     value = value + offset
 * @param dev sensor context pointer
 * @param offset count offset value used
 * @return Function result code
 */
upm_result_t light_set_offset(const light_context dev, float offset);

/**
 * Get sensor aref
 * @param dev sensor context pointer
 * @return Sensor ADC reference voltage
 */
float light_get_aref(const light_context dev);

/**
 * Get sensor scale
 * @param dev sensor context pointer
 * @return Sensor scale
 */
float light_get_scale(const light_context dev);

/**
 * Get sensor offset
 * @param dev sensor context pointer
 * @return Sensor offset
 */
float light_get_offset(const light_context dev);

/**
 * Read normalized value for sensor
 * @param dev sensor context pointer
 * @param *value Normalized value (0.0 -> 1.0)
 * @return Function result code
 */
upm_result_t light_get_normalized(const light_context dev, float *value);

/**
 * Read raw voltage from the sensor
 * @param dev sensor context pointer
 * @param *value Raw sensor voltage
 * @return Function result code
 */
upm_result_t light_get_raw_volts(const light_context dev, float *value);

/**
 * Read scaled/offset light value in lux
 * @param dev sensor context pointer
 * @param *value Adjusted sensor value in lux
 * @return Function result code
 */
upm_result_t light_get_lux(const light_context dev, float *value);
#ifdef __cplusplus
}
#endif
