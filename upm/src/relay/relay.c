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

#include "relay.h"

relay_context relay_init(int pin)
{
    relay_context dev = (relay_context)malloc(sizeof(struct _relay_context));

    if (dev == NULL)
        return NULL;

    dev->gpio = mraa_gpio_init(pin);

    if (dev->gpio == NULL)
    {
        free(dev);
        return NULL;
    }
    return dev;
}

void relay_close(relay_context dev)
{
    mraa_gpio_close(dev->gpio);

    free(dev);
}

upm_result_t relay_on(relay_context dev)
{
    mraa_gpio_write(dev->gpio, 1);
    return UPM_SUCCESS;
}

upm_result_t relay_off(relay_context dev)
{
    mraa_gpio_write(dev->gpio, 0);
    return UPM_SUCCESS;
}

bool relay_is_on(relay_context dev)
{
    int val;
    val = mraa_gpio_read(dev->gpio);
    if (val > 0)
        return true;
    else
        return false;
}

bool relay_is_off(relay_context dev)
{
    int val;
    val = mraa_gpio_read(dev->gpio);
    if (!val)
        return true;
    else
        return false;
}

