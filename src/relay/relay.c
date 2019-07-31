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

