/*
 * Author: Sisinty Sasmita Patra <sisinty.s.patra@intel.com>
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

#include "temperature.h"
#include "upm.h"

temperature_context temperature_init(int pin)
{
    temperature_context dev =
      (temperature_context)malloc(sizeof(struct _temperature_context));

    if (dev == NULL) return NULL;

    dev->aio = mraa_aio_init(pin);
    if(dev->aio == NULL)
    {
        free(dev);
        return NULL;
    }

    dev->m_aRes = (1 << mraa_aio_get_bit(dev->aio));

    return dev;
}

void temperature_close(temperature_context dev)
{
    mraa_aio_close(dev->aio);
    free(dev);
}

upm_result_t temperature_get_value(temperature_context dev,
                                   float* tempval)
{
    float val = 0.0;
    val = (float)mraa_aio_read(dev->aio);

    if (val == -1.0)
    {
        return UPM_ERROR_OPERATION_FAILED;
    }

    // From old C++ UPM code, needs rework!!!
    float r = ((float)dev->m_aRes - val) * 10000.0 / val;
    float t = 1.0 / (log(r / 10000.0) / 3975.0 + 1.0 / 298.15) -273.15;

    // Celsius
    *tempval = t;

    return UPM_SUCCESS;
}
