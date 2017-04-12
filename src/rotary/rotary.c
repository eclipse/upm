/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * 	   Sisinty Sasmita Patra <sisinty.s.patra@intel.com>
 *
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

#include <stdio.h>
#include <stdlib.h>

#include "rotary.h"

rotary_context rotary_init(int pin, float aRef)
{
    rotary_context dev = (rotary_context)malloc(sizeof(struct _rotary_context));

    if(dev == NULL) return NULL;

    dev->aio = mraa_aio_init(pin);

    if(dev->aio == NULL){
        free(dev);
        return NULL;
    }

    dev->m_aRef = aRef;

    // get adc bit range
    dev->m_aRes = (1 << mraa_aio_get_bit(dev->aio));

    return dev;
}

void rotary_close(rotary_context dev)
{
    mraa_aio_close(dev->aio);
    free(dev);
}

upm_result_t rotary_get_value_voltage (const rotary_context dev,
                                       float* volts)
{
    float val = 0.0;
    val = mraa_aio_read(dev->aio);
    *volts = (dev->m_aRef / dev->m_aRes) * (float)val;

    return UPM_SUCCESS;
}

upm_result_t rotary_get_value_angle (rotary_context dev, float* rotval)
{
    float val = 0.0;
    val = mraa_aio_read(dev->aio);

    // return degrees
    *rotval = val * (float)ROTARY_MAX_ANGLE / dev->m_aRes;

    return UPM_SUCCESS;
}
