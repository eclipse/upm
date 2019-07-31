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
#include <math.h>

#include "light.h"

light_context light_init(int16_t pin)
{
    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        return NULL;
    }

    light_context dev = (light_context) malloc(sizeof(struct _light_context));

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

void light_close(light_context dev)
{
    mraa_aio_close(dev->aio);
    free(dev);
}

upm_result_t light_set_aref(const light_context dev, float aref)
{
    dev->m_aRef = aref;
    return UPM_SUCCESS;
}

upm_result_t light_set_scale(const light_context dev, float scale)
{
    dev->m_scale = scale;
    return UPM_SUCCESS;
}

upm_result_t light_set_offset(const light_context dev, float offset)
{
    dev->m_offset = offset;
    return UPM_SUCCESS;
}

float light_get_aref(const light_context dev)
{
    return dev->m_aRef;
}

float light_get_scale(const light_context dev)
{
    return dev->m_scale;
}

float light_get_offset(const light_context dev)
{
    return dev->m_offset;
}

upm_result_t light_get_normalized(const light_context dev, float *value)
{
    *value = mraa_aio_read_float(dev->aio);
    if (*value < 0)
        return UPM_ERROR_OPERATION_FAILED;
    return UPM_SUCCESS;
}

upm_result_t light_get_raw_volts(const light_context dev, float *value)
{
    *value = mraa_aio_read_float(dev->aio);
    if (*value < 0)
        return UPM_ERROR_OPERATION_FAILED;

    /* Scale by the ADC reference voltage */
    *value *= dev->m_aRef;

    return UPM_SUCCESS;
}

upm_result_t light_get_lux(const light_context dev, float *value)
{
    *value = mraa_aio_read(dev->aio);
    if (*value < 0)
        return UPM_ERROR_OPERATION_FAILED;

    /* Get max adc value range 1023, 2047, 4095, etc... */
    float max_adc = (1 << mraa_aio_get_bit(dev->aio)) - 1;

    /* Convert the value to lux */
    *value = 10000.0/pow(((max_adc - *value) * 10.0 / *value)*15.0,4.0/3.0);

     /* Apply scale */
    *value *= dev->m_scale;

    /* Apply the offset in lux */
    *value += dev->m_offset;

    return UPM_SUCCESS;
}
