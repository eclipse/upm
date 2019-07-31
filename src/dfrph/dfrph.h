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
 * @file dfrph.h
 * @library dfrph
 * @brief C API for the DFRobot pH Sensors
 *
 * @include dfrph.c
 */

/**
 * device context
 */
typedef struct _dfrph_context {
    /* mraa aio pin context */
    mraa_aio_context aio;
    /* ADC reference */
    float m_aref;
    /* Raw count offset */
    float m_offset;
    /* Raw count scale */
    float m_scale;
} *dfrph_context;

/**
 * Initialize analog sensor
 * @param pin is Analog pin
 * @return sensor context as void pointer
 */
dfrph_context dfrph_init(int16_t pin);

/**
 * Analog sensor destructor
 * @param sensor context pointer deallocate memory
 */
void dfrph_close(dfrph_context dev);

/**
 * Set ADC reference voltage
 * @param dev sensor context pointer
 * @param aref ADC reference voltage
 * @return Function result code
 */
upm_result_t dfrph_set_aref(const dfrph_context dev, float aref);

/**
 *  Get sensor aref
 *  @param dev sensor context pointer
 *  @return Sensor ADC reference voltage
 */
float dfrph_get_aref(const dfrph_context dev);

/**
 * Set sensor offset.  This offset is applied to the return value:
 *     counts = counts + offset
 * @param dev sensor context pointer
 * @param offset count offset value used
 * @return Function result code
 */
upm_result_t dfrph_set_offset(const dfrph_context dev, float offset);

/**
 * Set sensor scale.  This scale is applied to the return value:
 *     counts = counts * scale
 * @param dev sensor context pointer
 * @param scale count scale value used
 * @return Function result code
 */
upm_result_t dfrph_set_scale(const dfrph_context dev, float scale);

/**
 * Get raw volts
 * @param dev sensor context pointer
 * @param volts Raw sensor voltage
 * @return Function result code
 */
upm_result_t dfrph_get_raw_volts(const dfrph_context dev, float *volts);

/**
 * Read value from sensor
 * @param dev sensor context pointer
 * @param value pointer to returned pH value from sensor
 * @return Function result code
 */
upm_result_t dfrph_get_ph(const dfrph_context dev, float *value);

#ifdef __cplusplus
}
#endif
