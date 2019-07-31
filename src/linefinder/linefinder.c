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

#include "linefinder.h"

linefinder_context linefinder_init(int pin)
{
    linefinder_context dev =
        (linefinder_context)malloc(sizeof(struct _linefinder_context));

    if (!dev)
        return NULL;

    dev->gpio = NULL;

    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        linefinder_close(dev);
        return NULL;
    }

    // MRAA contexts...
    if ( !(dev->gpio = mraa_gpio_init(pin)) )
    {
        printf("%s: mraa_gpio_init() failed\n",
               __FUNCTION__);
        linefinder_close(dev);
        return NULL;
    }

    mraa_gpio_dir(dev->gpio, MRAA_GPIO_IN);

    return dev;
}

void linefinder_close(linefinder_context dev)
{
    assert(dev != NULL);

    if (dev->gpio)
        mraa_gpio_close(dev->gpio);

    free(dev);
}

bool linefinder_white_detected(const linefinder_context dev)
{
    assert(dev != NULL);

    return (!mraa_gpio_read(dev->gpio) ? true : false);
}

bool linefinder_black_detected(const linefinder_context dev)
{
    assert(dev != NULL);

    return (mraa_gpio_read(dev->gpio) ? true : false);
}
