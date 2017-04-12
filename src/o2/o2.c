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

#include "o2.h"
o2_context o2_init(int16_t pin)
{
    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        return NULL;
    }

    o2_context dev = (o2_context)malloc(sizeof(struct _o2_context));

    if(dev == NULL) return NULL;

    /* Init aio pin */
    dev->aio = mraa_aio_init(pin);
    if(dev->aio == NULL) {
        free(dev);
        return NULL;
    }

    /* Set defaults */
    dev->m_aRef = 5.0;
    dev->m_offset = 0.0;
    dev->m_scale = 1.0;

    return dev;
}

void o2_close(o2_context dev)
{
    mraa_aio_close(dev->aio);
    free(dev);
}

upm_result_t o2_set_aref(const o2_context dev, float aref)
{
    dev->m_aRef = aref;
    return UPM_SUCCESS;
}

float o2_get_aref(const o2_context dev)
{
    return dev->m_aRef;
}

upm_result_t o2_set_offset(const o2_context dev, float offset)
{
    dev->m_offset = offset;
    return UPM_SUCCESS;
}

float o2_get_offset(const o2_context dev)
{
    return dev->m_offset;
}

upm_result_t o2_set_scale(const o2_context dev, float scale)
{
    dev->m_scale = scale;
    return UPM_SUCCESS;
}

float o2_get_scale(const o2_context dev)
{
    return dev->m_scale;
}

upm_result_t o2_get_counts(const o2_context dev, int *value)
{
    /* Read counts */
    *value = mraa_aio_read(dev->aio);
    if (*value < 0) return UPM_ERROR_OPERATION_FAILED;
    return UPM_SUCCESS;
}

upm_result_t o2_get_raw_volts(const o2_context dev, float *value)
{
    /* Read normalized adc value */
    *value = mraa_aio_read_float(dev->aio);
    if (*value < 0.0) return UPM_ERROR_OPERATION_FAILED;

    /* Convert normalized value to voltage via aRef */
    *value *= dev->m_aRef;

    return UPM_SUCCESS;
}

upm_result_t o2_get_value(const o2_context dev, float *value)
{
    /* Read normalized value */
    *value = mraa_aio_read_float(dev->aio);
    if (*value < 0.0)
        return UPM_ERROR_OPERATION_FAILED;

    /* Apply raw scale */
    *value *= dev->m_scale;

    /* Convert to %oxygen
       Datasheet for grove o2 shows a linear response for the sensor.  Assuming
       20.5% oxygen @ 25 celsius, with an gain = 1 + 12k/100 = 121, a
       dynamic range of 0->25% oxygen, and opamp rails of 0->3.3v (the grove o2
       sensor uses a high-accuracy 3.3v regulator),
     */
    *value *= 25 * dev->m_aRef/3.3;

    *value += dev->m_offset;

    return UPM_SUCCESS;
}

