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
 * @file mqx.h
 * @library mqx
 * @brief C API for the MQ Gas Sensors
 *
 * The mqx gas sensor driver can be used to read a wide range of sensors.  The
 * list below shows the currently supported sensors:
 *
 *      mq2: H2/LPG/CH4/CO/alcohol/smoke/propane/etc
 *          LPG/propane 200-5000 ppm
 *          butane 300-5000 ppm
 *          CH4 5000-2000 ppm
 *          H2 300-5000 ppm
 *          alcohol 100-2000 ppm
 *      mq3: Alcohol/benzine/CH4/hexane/LPG/CO .05-10 mg/L
 *      mq4: CNG (mostly methane) 200-10000 ppm
 *      mq5: H2/LPG/CH4/CO/alcohol 200-10000 ppm
 *      mq6: LPG (mostly propane/butane) 200-10000 ppm
 *      mq7: CO 20-2000 ppm
 *      mq8: H 100-10000 ppm
 *      mq9: CO/CH4/LPG 200-1000/10000/10000 ppm
 *
 * @include mqx.c
 */

/**
 * driver context
 */
typedef struct _mqx_context {
    /* mraa aio pin context */
    mraa_aio_context aio;
    /* Analog voltage reference */
    float m_aRef;
    /* Scale */
    float m_scale;
    /* Offset in sensor units */
    float m_offset;
} *mqx_context;

/**
 * Initialize analog sensor
 * @param pin Analog pin
 * @return sensor context
 */
mqx_context mqx_init(int16_t pin);

/**
 * Analog sensor destructor
 * @param sensor context pointer
 */
void mqx_close(mqx_context dev);

/**
 * Set ADC reference voltage
 * @param dev sensor context pointer
 * @param aref ADC reference voltage
 * @return Function result code
 */
upm_result_t mqx_set_aref(const mqx_context dev, float aref);

/**
 * Set sensor scale.  This scale is applied to the return value:
 *     counts = counts * scale
 * @param dev sensor context pointer
 * @param scale count scale value used
 * @return Function result code
 */
upm_result_t mqx_set_scale(const mqx_context dev, float scale);

/**
 * Set sensor offset.  This offset is applied to the return value:
 *     counts = counts + offset
 * @param dev sensor context pointer
 * @param offset count offset value used
 * @return Function result code
 */
upm_result_t mqx_set_offset(const mqx_context dev, float offset);

/**
 * Get sensor aref
 * @param dev sensor context pointer
 * @return Sensor ADC reference voltage
 */
float mqx_get_aref(const mqx_context dev);

/**
 * Get sensor scale
 * @param dev sensor context pointer
 * @return Sensor scale
 */
float mqx_get_scale(const mqx_context dev);

/**
 * Get sensor offset
 * @param dev sensor context pointer
 * @return Sensor offset
 */
float mqx_get_offset(const mqx_context dev);

/**
 * Read normalized value for sensor
 * @param dev sensor context pointer
 * @param *value Normalized value (0.0 -> 1.0)
 * @return Function result code
 */
upm_result_t mqx_get_normalized(const mqx_context dev, float *value);

/**
 * Read raw voltage from the sensor
 * @param dev sensor context pointer
 * @param *value Raw sensor voltage
 * @return Function result code
 */
upm_result_t mqx_get_raw_volts(const mqx_context dev, float *value);

/**
 * Read scaled/offset voltage from the sensor
 * @param dev sensor context pointer
 * @param *value Adjusted sensor voltage
 * @return Function result code
 */
upm_result_t mqx_get_volts(const mqx_context dev, float *value);
#ifdef __cplusplus
}
#endif
