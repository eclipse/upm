/*
 * Author: Jon Trulson <jtrulson@ics.com>
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
