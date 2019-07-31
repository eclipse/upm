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
#include "yg1006.h"

yg1006_context yg1006_init(unsigned int pin)
{
    yg1006_context dev =
        (yg1006_context)malloc(sizeof(struct _yg1006_context));

    if (!dev)
        return NULL;

    dev->gpio = NULL;

    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        yg1006_close(dev);
        return NULL;
    }

    // initialize the MRAA context

    if (!(dev->gpio = mraa_gpio_init(pin)))
    {
        printf("%s: mraa_gpio_init() failed.\n", __FUNCTION__);
        yg1006_close(dev);
        return NULL;
    }

    mraa_gpio_dir(dev->gpio, MRAA_GPIO_IN);

    return dev;
}

void yg1006_close(yg1006_context dev)
{
    assert(dev != NULL);

    if (dev->gpio)
        mraa_gpio_close(dev->gpio);

    free(dev);
}

bool yg1006_flame_detected(const yg1006_context dev)
{
    assert(dev != NULL);

    // gpio is low when a flame is detected
    return (mraa_gpio_read(dev->gpio) ? false : true);
}
