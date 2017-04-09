/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
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

#include <assert.h>
#include <string.h>
#include "lm35.h"

lm35_context lm35_init(int pin, float aref)
{
    lm35_context dev =
        (lm35_context)malloc(sizeof(struct _lm35_context));

    if (!dev)
        return NULL;

    memset((void *)dev, 0, sizeof(struct _lm35_context));

    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        lm35_close(dev);
        return NULL;
    }

    // initialize the MRAA context

    if (!(dev->aio = mraa_aio_init(pin)))
    {
        printf("%s: mraa_aio_init failed.\n", __FUNCTION__);
        lm35_close(dev);

        return NULL;
    }

    dev->ares = (float)((1 << mraa_aio_get_bit(dev->aio)) - 1);
    dev->aref = aref;
    dev->scale = 1.0;
    dev->offset = 0.0;

    return dev;
}

void lm35_close(lm35_context dev)
{
    assert(dev != NULL);

    if (dev->aio)
        mraa_aio_close(dev->aio);

    free(dev);
}

upm_result_t lm35_get_temperature(const lm35_context dev, float *temperature)
{
    assert(dev != NULL);

    int val = mraa_aio_read(dev->aio);
    if (val < 0)
    {
        printf("%s: mraa_aio_read() failed\n", __FUNCTION__);
        *temperature = 0.0;
        return UPM_ERROR_OPERATION_FAILED;
    }

    // convert to mV
    float mV = ((float)val * (dev->aref / dev->ares) * 1000.0);

    // 10mV/degree C
    *temperature = mV / 10.0;
    *temperature = *temperature * dev->scale + (dev->offset * dev->scale);
    return UPM_SUCCESS;
}

void lm35_set_offset(const lm35_context dev, float offset)
{
    assert(dev != NULL);

    dev->offset = offset;
}

void lm35_set_scale(const lm35_context dev, float scale)
{
    assert(dev != NULL);

    dev->scale = scale;
}
