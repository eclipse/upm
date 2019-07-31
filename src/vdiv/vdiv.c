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

#include "vdiv.h"

vdiv_context vdiv_init(int16_t pin, float voltage_ref)
{
    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        return NULL;
    }

    vdiv_context dev = (vdiv_context)malloc(sizeof(struct _vdiv_context));

    if (dev == NULL) return NULL;

    /* Init aio pin */
    dev->aio = mraa_aio_init(pin);

    if (dev->aio == NULL) {
        free(dev);
        return NULL;
    }

    /* Set defaults */
    dev->m_aRef = 5.0;
    dev->m_scale = 1.0;
    dev->m_offset = 0.0;
    dev->m_vdiv_sw = 3;

    return dev;
}

void vdiv_close(vdiv_context dev)
{
    mraa_aio_close(dev->aio);
    free(dev);
}

upm_result_t vdiv_set_scale(const vdiv_context dev, float scale)
{
    dev->m_scale = scale;
    return UPM_SUCCESS;
}

upm_result_t vdiv_set_offset(const vdiv_context dev, float offset)
{
    dev->m_offset = offset;
    return UPM_SUCCESS;
}

float vdiv_get_scale(const vdiv_context dev)
{
    return dev->m_scale;
}

float vdiv_get_offset(const vdiv_context dev)
{
    return dev->m_offset;
}

upm_result_t vdiv_set_divsw(const vdiv_context dev, int vdiv_sw)
{
    dev->m_vdiv_sw = vdiv_sw;
    return UPM_SUCCESS;
}

int vdiv_get_divsw(const vdiv_context dev)
{
    return dev->m_vdiv_sw;
}

upm_result_t vdiv_get_raw_volts(const vdiv_context dev, float *value)
{
    *value = mraa_aio_read_float(dev->aio);
    if (*value < 0)
        return UPM_ERROR_OPERATION_FAILED;

    /* Scale by the ADC reference voltage */
    *value *= dev->m_aRef;

    return UPM_SUCCESS;
}

upm_result_t vdiv_get_computed_volts(const vdiv_context dev, float *value)
{
    // JET - this is wrong.

    *value = mraa_aio_read_float(dev->aio);
    if (*value < 0)
        return UPM_ERROR_OPERATION_FAILED;

     /* Apply raw scale */
    *value *= dev->m_scale;

    /* Scale to the ADC referecen then to vdiv gain */
    *value *= dev->m_aRef * dev->m_vdiv_sw;

    /* Apply the offset in volts */
    *value += dev->m_offset;

    return UPM_SUCCESS;
}
