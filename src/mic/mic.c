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

#include "mic.h"

mic_context upm_mic_init(int pin)
{
    mic_context dev = (mic_context)malloc(sizeof(struct _mic_context));

    if(dev == NULL) return NULL;

    dev->analog_pin = pin;
    dev->aio = mraa_aio_init(dev->analog_pin);

    if(dev->aio == NULL)
    {
        printf("unable to initialize the AIO pin");
        free(dev);
        return NULL;
    }

    return dev;
}

void mic_close(mic_context dev)
{
    mraa_aio_close(dev->aio);
    free(dev);
}

upm_result_t mic_get_value(mic_context dev, float *micval)
{
    *micval =  mraa_aio_read(dev->aio);
    return UPM_SUCCESS;
}
