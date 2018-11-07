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

#include <string.h>
#include <stdlib.h>

#include "joystick12.h"

joystick12_context joystick12_init(int16_t apin_x, int16_t apin_y)
{
    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        return NULL;
    }

    joystick12_context dev =
      (joystick12_context) malloc(sizeof(struct _joystick12_context));

    if(dev == NULL) return NULL;

    /* Init analog pins */
    dev->ai_x = mraa_aio_init(apin_x);
    dev->ai_y = mraa_aio_init(apin_y);

    /* Set the ref, zero the offset */
    dev->m_count_offset_x = 0.0;
    dev->m_count_scale_x = 2.0;
    dev->m_count_offset_y = 0.0;
    dev->m_count_scale_y = 2.0;

    if((dev->ai_x == NULL) || (dev->ai_y == NULL))
    {
        free(dev);
        return NULL;
    }

    return dev;
}

void joystick12_close(joystick12_context dev)
{
    mraa_aio_close(dev->ai_x);
    mraa_aio_close(dev->ai_y);
    free(dev);
}

upm_result_t joystick12_read_data(const joystick12_context dev, int* values)
{
    /* Throw away first read */
    mraa_aio_read(dev->ai_x);
    values[0] = mraa_aio_read(dev->ai_x);
    /* Throw away first read */
    mraa_aio_read(dev->ai_y);
    values[1] = mraa_aio_read(dev->ai_y);

    if (values[0] < 0 || values[1] < 0)
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t joystick12_set_offset_x(const joystick12_context dev, float offset)
{
    dev->m_count_offset_x = offset;
    return UPM_SUCCESS;
}

upm_result_t joystick12_set_offset_y(const joystick12_context dev, float offset)
{
    dev->m_count_offset_y = offset;
    return UPM_SUCCESS;
}

upm_result_t joystick12_set_scale_x(const joystick12_context dev, float scale)
{
    dev->m_count_scale_x = scale;
    return UPM_SUCCESS;
}

upm_result_t joystick12_set_scale_y(const joystick12_context dev, float scale)
{
    dev->m_count_scale_y = scale;
    return UPM_SUCCESS;
}

upm_result_t joystick12_get_value_x(const joystick12_context dev, float *value)
{
    int counts[2] = {0,0};

    /* Read counts from the generic read method */
    joystick12_read_data(dev, counts);

    /* Get max adc value range 1023, 2047, 4095, etc... */
    float max_adc = (1 << mraa_aio_get_bit(dev->ai_x)) - 1;

    /* Apply raw offset */
    *value = counts[0] + dev->m_count_offset_x;

    // Scale to +/- 1.0
    *value =  -2.0 * (((max_adc/2.0 - *value))/max_adc * dev->m_count_scale_x);

    // Clip to +/- 1.0
    if (*value > 1.0) *value = 1.0;
    else if (*value < -1.0) *value = -1.0;

    return UPM_SUCCESS;
}

upm_result_t joystick12_get_value_y(const joystick12_context dev, float *value)
{
    int counts[2] = {0,0};

    /* Read counts from the generic read method */
    joystick12_read_data(dev, counts);

    /* Get max adc value range 1023, 2047, 4095, etc... */
    float max_adc = (1 << mraa_aio_get_bit(dev->ai_y)) - 1;

    /* Apply raw offset */
    *value = counts[1] + dev->m_count_offset_y;

    // Scale to +/- 1.0
    *value =  -2.0 * (((max_adc/2.0 - *value))/max_adc * dev->m_count_scale_y);

    // Clip to +/- 1.0
    if (*value > 1.0) *value = 1.0;
    else if (*value < -1.0) *value = -1.0;

    return UPM_SUCCESS;
}

upm_result_t joystick12_zero(const joystick12_context dev)
{
    int counts[2] = {0, 0};

    /* Read counts from the generic read method */
    joystick12_read_data(dev, counts);

    /* Get max adc value range 1023, 2047, 4095, etc... */
    float max_adc = (1 << mraa_aio_get_bit(dev->ai_y)) - 1;

    dev->m_count_offset_x = max_adc/2.0 - counts[0];
    dev->m_count_offset_y = max_adc/2.0 - counts[1];

    return UPM_SUCCESS;
}

upm_result_t joystick12_calibrate_x(const joystick12_context dev)
{
    int counts[2] = {0, 0};

    /* Read counts from the generic read method */
    joystick12_read_data(dev, counts);

    /* Get max adc value range 1023, 2047, 4095, etc... */
    float max_adc = (1 << mraa_aio_get_bit(dev->ai_y)) - 1;

    float fcounts = counts[0];

    /* no need to scale if counts is a max or min value */
    if ((fcounts == 0) || (fcounts == max_adc)) return UPM_SUCCESS;

    dev->m_count_scale_x =
      1.0 / (1.0 - 2.0 * fcounts/max_adc);
    dev->m_count_scale_x = fabs(dev->m_count_scale_x);

    return UPM_SUCCESS;
}

upm_result_t joystick12_calibrate_y(const joystick12_context dev)
{
    int counts[2] = {0, 0};

    /* Read counts from the generic read method */
    joystick12_read_data(dev, counts);

    /* Get max adc value range 1023, 2047, 4095, etc... */
    float max_adc = (1 << mraa_aio_get_bit(dev->ai_y)) - 1;

    float fcounts = counts[1];

    /* no need to scale if counts is a max or min value */
    if ((fcounts == 0) || (fcounts == max_adc)) return UPM_SUCCESS;

    dev->m_count_scale_y =
      1.0 / (1.0 - 2.0 * fcounts/max_adc);
    dev->m_count_scale_y = fabs(dev->m_count_scale_y);

    return UPM_SUCCESS;
}
