/*
 * Author:
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

#include "dfrph.h"

dfrph_context dfrph_init(int16_t pin)
{
    dfrph_context dev = (dfrph_context) malloc(sizeof(analog_sensor_t));

    if(dev == NULL) return NULL;

    /* Init aio pin */
    dev->aio = mraa_aio_init(pin);

    /* Set the ref, zero the offset */
    dev->aref = 5.0;
    dev->offset = 0.0;
    dev->scale = 1.0;

    if(dev->aio == NULL) {
        free(dev);
        return NULL;
    }

    return dev;
}

void dfrph_close(dfrph_context dev)
{
    mraa_aio_close(dev->aio);
    free(dev);
}

upm_result_t dfrph_set_offset(const dfrph_context dev, float offset)
{
    dev->offset = offset;
    return UPM_SUCCESS;
}

upm_result_t dfrph_set_scale(const dfrph_context dev, float scale)
{
    dev->scale = scale;
    return UPM_SUCCESS;
}

upm_result_t dfrph_get_ph(const dfrph_context dev, float *value)
{
    /* Throw away first analog read (can be slightly off) */
    mraa_aio_read(dev->aio);
    /* Read normalized */
    *value = mraa_aio_read_float(dev->aio);

    /* Scale by aref */
    *value *= dev->aref/4;

    /* Apply user scale */
    *value *= dev->scale;

    /* Convert to pH */
    *value *= 14.0;

    /* Apply the offset (in pH) */
    *value += dev->offset;

    return UPM_SUCCESS;
}
