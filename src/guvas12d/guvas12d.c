/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */


#include <assert.h>
#include <string.h>
#include <upm_utilities.h>

#include "guvas12d.h"

guvas12d_context guvas12d_init(int pin, float aref)
{
    guvas12d_context dev =
        (guvas12d_context)malloc(sizeof(struct _guvas12d_context));

    if (!dev)
        return NULL;

    memset((void *)dev, 0, sizeof(struct _guvas12d_context));

    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        guvas12d_close(dev);
        return NULL;
    }

    // initialize the MRAA context

    if (!(dev->aio = mraa_aio_init(pin)))
    {
        printf("%s: mraa_aio_init failed.\n", __FUNCTION__);
        guvas12d_close(dev);

        return NULL;
    }

    dev->ares = (float)((1 << mraa_aio_get_bit(dev->aio)) - 1);
    dev->aref = aref;
    dev->scale = 1.0;
    dev->offset = 0.0;

    return dev;
}

void guvas12d_close(guvas12d_context dev)
{
    assert(dev != NULL);

    if (dev->aio)
        mraa_aio_close(dev->aio);

    free(dev);
}

upm_result_t guvas12d_get_volts(const guvas12d_context dev, float *volts)
{
    assert(dev != NULL);

    int val;

    val = mraa_aio_read(dev->aio);
    if (val < 0)
    {
        printf("%s: mraa_aio_read() failed\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }
    *volts = (float)val * (dev->aref / dev->ares);

    return UPM_SUCCESS;
}

upm_result_t guvas12d_get_intensity(const guvas12d_context dev,
                                    float *intensity)
{
    assert(dev != NULL);

    float volts;
    if (guvas12d_get_volts(dev, &volts))
    {
        printf("%s: guvas12d_get_volts() failed\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    // Seeed magic number 307.0
    *intensity = volts * 307.0;

    *intensity = *intensity * dev->scale + (dev->offset * dev->scale);

    return UPM_SUCCESS;
}

void guvas12d_set_offset(const guvas12d_context dev, float offset)
{
    assert(dev != NULL);

    dev->offset = offset;
}

void guvas12d_set_scale(const guvas12d_context dev, float scale)
{
    assert(dev != NULL);

    dev->scale = scale;
}
