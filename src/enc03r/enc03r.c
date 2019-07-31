/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014-2017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "enc03r.h"
#include "upm_utilities.h"

enc03r_context enc03r_init(int pin, float aref)
{
    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        return NULL;
    }

    enc03r_context dev =
        (enc03r_context)malloc(sizeof(struct _enc03r_context));

    if (!dev)
        return NULL;

    // zero out context
    memset((void *)dev, 0, sizeof(struct _enc03r_context));

    dev->a_ref = aref;

    if ( !(dev->aio = mraa_aio_init(pin)) )
    {
        printf("%s: mraa_aio_init() failed.\n", __FUNCTION__);
        enc03r_close(dev);
        return NULL;
    }

    dev->offset = 0.0;
    dev->scale = 1.0;

    dev->a_res = (float)(1 << mraa_aio_get_bit(dev->aio)) - 1.0;

    dev->calibrationValue = 0.0;

    return dev;
}

void enc03r_close(enc03r_context dev)
{
    assert(dev != NULL);

    if (dev->aio)
        mraa_aio_close(dev->aio);

    free(dev);
}

upm_result_t enc03r_update(enc03r_context dev)
{
    assert(dev != NULL);

    float val = (float)mraa_aio_read(dev->aio);
    if (val < 0)
    {
        printf("%s: mraa_aio_read() failed\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    dev->normalized = val / dev->a_res;

    // from seeed studio example
    dev->angular_velocity =
        ((val - dev->calibrationValue) * (dev->a_ref * 1000.0)
         / dev->a_res / 0.67);

    return UPM_SUCCESS;
}

upm_result_t enc03r_calibrate(const enc03r_context dev,
                                   unsigned int samples)
{
    assert(dev != NULL);

    float total = 0.0;

    for (unsigned int i=0; i<samples; i++)
    {
        int val = mraa_aio_read(dev->aio);
        if (val < 0)
        {
            printf("%s: mraa_aio_read() failed\n", __FUNCTION__);
            return UPM_ERROR_OPERATION_FAILED;
        }

        total += (float)val;
        upm_delay_ms(2);
    }

    dev->calibrationValue = total / (float)samples;

    return UPM_SUCCESS;
}

float enc03r_calibration_value(const enc03r_context dev)
{
    assert(dev != NULL);

    return dev->calibrationValue;
}

float enc03r_angular_velocity(const enc03r_context dev)
{
    assert(dev != NULL);

    return dev->angular_velocity * dev->scale + (dev->offset * dev->scale);
}

void enc03r_set_offset(const enc03r_context dev, float offset)
{
    assert(dev != NULL);

    dev->offset = offset;
}

void enc03r_set_scale(const enc03r_context dev, float scale)
{
    assert(dev != NULL);

    dev->scale = scale;
}

float enc03r_get_normalized(const enc03r_context dev)
{
    assert(dev != NULL);

    return dev->normalized;
}
