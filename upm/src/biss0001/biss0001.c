/*
 * Author: Jon Trulson <jtrulson@ics.com>
 *         Zion Orent <zorent@ics.com>
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
