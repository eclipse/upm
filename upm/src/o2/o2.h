/*
 * Author: Noel Eck <noel.eck@intel.com>
 * Copyright (c) 2015 Intel Corporation.
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

#pragma once

#include "upm.h"
#include "mraa/aio.h"


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file o2.h
 * @library o2
 * @brief C API for the O2 Oxygen Gas Sensor
 *
 * @include o2.c
 */

/**
 * device context
 */
typedef struct _o2_context {
    /* mraa aio pin context */
    mraa_aio_context aio;
    /* Analog voltage reference */
    float m_aRef;
    /* Scale */
    float m_scale;
    /* Offset in sensor units */
    float m_offset;
} *o2_context;

/**
 * Initialize analog sensor
 * @param pin is Analog pin
 * @return sensor context as void pointer
 */
o2_context o2_init(int16_t pin);

/**
 * Analog sensor destructor
 * @param sensor context pointer deallocate memory
 */
void o2_close(o2_context dev);

/**
 * Set sensor adc vref.
 * @param dev sensor context pointer
 * @param aref Voltage reference routed to ADC ref pin
 * @return Function result code
 */
upm_result_t o2_set_aref(const o2_context dev, float aref);

/**
 * Get sensor adc vref
 * @param dev sensor context pointer
 * @return Current adc vref
 */
float o2_get_aref(const o2_context dev);

/**
 * Set sensor offset.  This offset is applied to the return value:
 *     counts = counts * scale + offset * scale
 * @param dev sensor context pointer
 * @param offset count offset value used
 * @return Function result code
 */
upm_result_t o2_set_offset(const o2_context dev, float offset);

/**
 * Get sensor offset
 * @param dev sensor context pointer
 * @return Current sensor offset
 */
float o2_get_offset(const o2_context dev);

/**
 * Set sensor scale.  This scale is applied to the return value:
 *     counts = counts * scale + offset * scale
 * @param dev sensor context pointer
 * @param scale count scale value used
 * @return Function result code
 */
upm_result_t o2_set_scale(const o2_context dev, float scale);

/**
 * Get sensor scale
 * @param dev sensor context pointer
 * @return Current sensor scale
 */
float o2_get_scale(const o2_context dev);

/**
 * Read raw adc counts from sensor
 * @param dev sensor context pointer
 * @param *value Raw adc value
 * @return Function result code
 */
upm_result_t o2_get_counts(const o2_context dev, int *value);

/**
 * Read voltage from sensor
 * @param dev sensor context pointer
 * @param *value Voltage (v)
 * @return Function result code
 */
upm_result_t o2_get_raw_volts(const o2_context dev, float *value);

/**
 * Read value from sensor
 * @param dev sensor context pointer
 * @param *value O2 percentage
 * @return Function result code
 */
upm_result_t o2_get_value(const o2_context dev, float *value);

#ifdef __cplusplus
}
#endif
