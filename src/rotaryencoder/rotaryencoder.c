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
#include "rotaryencoder.h"


// our ISR
static void rotaryencoder_isr(void *ctx)
{
    assert (ctx != NULL);

    rotaryencoder_context dev = (rotaryencoder_context)ctx;

    if (mraa_gpio_read(dev->gpioA))
    {
        if (mraa_gpio_read(dev->gpioB))
            dev->position++;      // CW
        else
            dev->position--;      // CCW
    }
}

rotaryencoder_context rotaryencoder_init(int pin_a, int pin_b)
{
    rotaryencoder_context dev =
        (rotaryencoder_context)malloc(sizeof(struct _rotaryencoder_context));

    if (!dev)
        return NULL;

    dev->gpioA = NULL;
    dev->gpioB = NULL;

    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        rotaryencoder_close(dev);
        return NULL;
    }

    // MRAA contexts...
    if ( !(dev->gpioA = mraa_gpio_init(pin_a)) )
    {
        printf("%s: mraa_gpio_init(pin_a) failed\n",
               __FUNCTION__);
        rotaryencoder_close(dev);
        return NULL;
    }

    mraa_gpio_dir(dev->gpioA, MRAA_GPIO_IN);

    if ( !(dev->gpioB = mraa_gpio_init(pin_b)) )
    {
        printf("%s: mraa_gpio_init(pin_b) failed\n",
               __FUNCTION__);
        rotaryencoder_close(dev);
        return NULL;
    }

    mraa_gpio_dir(dev->gpioB, MRAA_GPIO_IN);

    dev->position = 0;

    // setup the ISR

    // We would prefer to use MRAA_GPIO_EDGE_BOTH for better resolution,
    // but that does not appear to be supported universally
    if (mraa_gpio_isr(dev->gpioA, MRAA_GPIO_EDGE_RISING,
                      &rotaryencoder_isr, dev))
    {
        printf("%s: mraa_gpio_isr() failed\n",
               __FUNCTION__);
        rotaryencoder_close(dev);
        return NULL;
    }

    return dev;
}

void rotaryencoder_close(rotaryencoder_context dev)
{
    assert (dev != NULL);

    if (dev->gpioA)
    {
        mraa_gpio_isr_exit(dev->gpioA);
        mraa_gpio_close(dev->gpioA);
    }

    if (dev->gpioB)
        mraa_gpio_close(dev->gpioB);

    free(dev);
}

void rotaryencoder_set_position(const rotaryencoder_context dev, int count)
{
    assert (dev != NULL);

    dev->position = count;
}

int rotaryencoder_get_position(const rotaryencoder_context dev)
{
    assert (dev != NULL);

    return dev->position;
}
