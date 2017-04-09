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
 * @file vdiv.h
 * @library vdiv
 * @brief C API for Voltage Dividers
 *
 * @include vdiv.c
 */

/**
 * device context
 */
typedef struct _vdiv_context {
    /* mraa aio pin context */
    mraa_aio_context aio;
    /* ADC voltage reference */
    float m_aRef;
    /* Scale */
    float m_scale;
    /* Offset in sensor units */
    float m_offset;
    /* Offset in sensor units */
    int m_vdiv_sw;
} *vdiv_context;


/**
 * Initialize analog sensor
 * @param pin is Analog pin
 * @return sensor context as void pointer
 */
vdiv_context vdiv_init(int16_t pin, float voltage_ref);

/**
 * Analog sensor destructor
 * @param sensor context pointer deallocate memory
 */
void vdiv_close(vdiv_context dev);

/**
 * Set sensor scale.  This scale is applied to the return value:
 *     counts = counts * scale
 * @param dev sensor context pointer
 * @param scale count scale value used
 * @return Function result code
 */
upm_result_t vdiv_set_scale(const vdiv_context dev, float scale);

/**
 * Set sensor offset.  This offset is applied to the return value:
 *     counts = counts + offset
 * @param dev sensor context pointer
 * @param offset count offset value used
 * @return Function result code
 */
upm_result_t vdiv_set_offset(const vdiv_context dev, float offset);

/**
 * Get sensor scale
 * @param dev sensor context pointer
 * @return Sensor scale
 */
float vdiv_get_scale(const vdiv_context dev);

/**
 * Get sensor offset
 * @param dev sensor context pointer
 * @return Sensor offset
 */
float vdiv_get_offset(const vdiv_context dev);

/**
 * Set sensor divide switch value
 * @param dev sensor context pointer
 * @param vdiv_sw Divide switch value
 * @return Function result code
 */
upm_result_t vdiv_set_divsw(const vdiv_context dev, int vdiv_sw);

/**
 * Get divide switch value
 * @param dev sensor context pointer
 * @return Sensor divide switch value
 */
int vdiv_get_divsw(const vdiv_context dev);

/**
 * Read raw voltage from the sensor
 * @param dev sensor context pointer
 * @param *value Raw sensor voltage
 * @return Function result code
 */
upm_result_t vdiv_get_raw_volts(const vdiv_context dev, float *value);

/**
 * Gets the true voltage value from the sensor.  Voltage divides by 3 or 10,
 * depending on the toggle switch on the board.  The dynamic range of the
 * vdiv sensor is listed below.
 * 
 *  ADC Ref  SW   max VOL in
 *  ------- ---   ----------
 *    3.3v    3       8.5v
 *    3.3v   10      28.4v
 *    5.0v    3      12.9v
 *    5.0v   10      43.0v
 *
 * @param dev sensor context pointer
 * @param *value Voltage (v)
 * @return Function result code
 */
upm_result_t vdiv_get_computed_volts(const vdiv_context dev, float *value);

#ifdef __cplusplus
}
#endif
