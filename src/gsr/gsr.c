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

#include <string.h>
#include <stdlib.h>

#include "gsr.h"

gsr_context gsr_init(int16_t pin)
{
    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        return NULL;
    }

    gsr_context dev = (gsr_context) malloc(sizeof(struct _gsr_context));

    if (dev == NULL)
      return NULL;

    /* Init aio pin */
    dev->aio = mraa_aio_init(pin);

    if (dev->aio == NULL) {
        free(dev);
        return NULL;
    }

    /* Set the ADC ref, scale, and offset defaults */
    dev->m_aRef = 5.0;
    dev->m_scale = 1.0;
    dev->m_offset = 0.0;

    return dev;
}

void gsr_close(gsr_context dev)
{
    mraa_aio_close(dev->aio);
    free(dev);
}

upm_result_t gsr_set_aref(const gsr_context dev, float aref)
{
    dev->m_aRef = aref;
    return UPM_SUCCESS;
}

upm_result_t gsr_set_scale(const gsr_context dev, float scale)
{
    dev->m_scale = scale;
    return UPM_SUCCESS;
}

upm_result_t gsr_set_offset(const gsr_context dev, float offset)
{
    dev->m_offset = offset;
    return UPM_SUCCESS;
}

float gsr_get_aref(const gsr_context dev)
{
    return dev->m_aRef;
}

float gsr_get_scale(const gsr_context dev)
{
    return dev->m_scale;
}

float gsr_get_offset(const gsr_context dev)
{
    return dev->m_offset;
}

upm_result_t gsr_get_normalized(const gsr_context dev, float *value)
{
    *value = mraa_aio_read_float(dev->aio);
    if (*value < 0)
        return UPM_ERROR_OPERATION_FAILED;
    return UPM_SUCCESS;
}

upm_result_t gsr_get_raw_volts(const gsr_context dev, float *value)
{
    *value = mraa_aio_read_float(dev->aio);
    if (*value < 0)
        return UPM_ERROR_OPERATION_FAILED;

    /* Scale by the ADC reference voltage */
    *value *= dev->m_aRef;

    return UPM_SUCCESS;
}

upm_result_t gsr_get_volts(const gsr_context dev, float *value)
{
    *value = mraa_aio_read_float(dev->aio);
    if (*value < 0)
        return UPM_ERROR_OPERATION_FAILED;

     /* Apply raw scale */
    *value *= dev->m_scale;

     /* Scale to aRef */
    *value *= dev->m_aRef;

    /* Apply the offset in volts */
    *value += dev->m_offset;

    return UPM_SUCCESS;
}
