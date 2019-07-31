/*
 * Author: Jon Trulson <jtrulson@ics.com>
 *         Zion Orent <zorent@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <assert.h>
#include "biss0001.h"

biss0001_context biss0001_init(unsigned int pin)
{
    biss0001_context dev =
        (biss0001_context)malloc(sizeof(struct _biss0001_context));

    if (!dev)
        return NULL;

    dev->gpio = NULL;

    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        biss0001_close(dev);
        return NULL;
    }

    // initialize the MRAA context

    if (!(dev->gpio = mraa_gpio_init(pin)))
    {
        printf("%s: mraa_gpio_init() failed.\n", __FUNCTION__);
        biss0001_close(dev);
        return NULL;
    }

    mraa_gpio_dir(dev->gpio, MRAA_GPIO_IN);

    return dev;
}

void biss0001_close(biss0001_context dev)
{
    assert(dev != NULL);

    if (dev->gpio)
        mraa_gpio_close(dev->gpio);

    free(dev);
}

bool biss0001_motion_detected(const biss0001_context dev)
{
    assert(dev != NULL);

    return (mraa_gpio_read(dev->gpio) ? true : false);
}
