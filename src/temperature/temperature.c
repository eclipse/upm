/*
 * Author: Sisinty Sasmita Patra <sisinty.s.patra@intel.com>
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
