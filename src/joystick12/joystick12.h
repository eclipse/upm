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
 * @file joystick12.h
 * @library joystick12
 * @brief C API for Analog 2-Axis Joysticks
 *
 * @include joystick12.c
 */

/**
 * device context
 */
typedef struct _joystick12_context {
    /* mraa ai_x pin context */
    mraa_aio_context ai_x;
    /* mraa ai_y pin context */
    mraa_aio_context ai_y;
    /* Raw count offset - x axis */
    float m_count_offset_x;
    /* Raw count offset - y axis */
    float m_count_offset_y;
    /* Raw count scale - x axis */
    float m_count_scale_x;
    /* Raw count scale - y axis */
    float m_count_scale_y;
} *joystick12_context;

/**
 * Initialize analog sensor
 * @param pin is Analog pin
 * @return sensor context as void pointer
 */
joystick12_context joystick12_init(int16_t apin_x, int16_t apin_y);

/**
 * Analog sensor destructor
 * @param sensor context pointer deallocate memory
 */
void joystick12_close(joystick12_context dev);

/**
 * Generic read joystick data method
 * @param dev sensor device context pointer
 * @param value pointer to an int[2] in which data is returned
 */
upm_result_t joystick12_read_data(const joystick12_context dev, int* values);

/**
 * Set sensor offset.  This offset is applied to the return value:
 *     counts = counts + offset
 * @param dev sensor context pointer
 * @param offset count offset value used
 * @return Function result code
 */
upm_result_t joystick12_set_offset_x(const joystick12_context dev,
                                     float offset);

/**
 * Set sensor offset.  This offset is applied to the return value:
 *     counts = counts + offset
 * @param dev sensor context pointer
 * @param offset count offset value used
 * @return Function result code
 */
upm_result_t joystick12_set_offset_y(const joystick12_context dev,
                                     float offset);

/**
 * Set sensor scale.  This scale is applied to the return value:
 *     counts = counts * scale
 * @param dev sensor context pointer
 * @param scale count scale value used
 * @return Function result code
 */
upm_result_t joystick12_set_scale_x(const joystick12_context dev, float scale);

/**
 * Set sensor scale.  This scale is applied to the return value:
 *     counts = counts * scale
 * @param dev sensor context pointer
 * @param scale count scale value used
 * @return Function result code
 */
upm_result_t joystick12_set_scale_y(const joystick12_context dev, float scale);

/**
 * Read value from sensor
 * @param dev sensor context pointer
 * @param *value normalized value from sensor, units depend on unit enum
 * @param unit Enum which specifies units returned in *value
 * @return Function result code
 */
upm_result_t joystick12_get_value_x(const joystick12_context dev, float *value);

/**
 * Read value from sensor
 * @param dev sensor context pointer
 * @param *value normalized value from sensor, units depend on unit enum
 * @param unit Enum which specifies units returned in *value
 * @return Function result code
 */
upm_result_t joystick12_get_value_y(const joystick12_context dev, float *value);

/**
 * Zero out the sensor x and y axis
 * @param dev sensor context pointer
 * @return Function result code
 */
upm_result_t joystick12_zero(const joystick12_context dev);

/**
 * Set x scale.  Move joystick to min or max x and call calibrate
 * @param dev sensor context pointer
 * @return Function result code
 */
upm_result_t joystick12_calibrate_x(const joystick12_context dev);

/**
 * Set y scale.  Move joystick to min or max y and call calibrate
 * @param dev sensor context pointer
 * @return Function result code
 */
upm_result_t joystick12_calibrate_y(const joystick12_context dev);

#ifdef __cplusplus
}
#endif
