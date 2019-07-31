/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * 	   Sisinty Sasmita Patra <sisinty.s.patra@intel.com>
 *
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
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
