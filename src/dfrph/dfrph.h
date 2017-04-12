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
