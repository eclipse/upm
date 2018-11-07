/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
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
#include <string.h>
#include <assert.h>

#include "wfs.h"

// interrupt handler
static void wfs_flow_isr(void *ctx)
{
    assert(ctx != NULL);

    wfs_context dev = (wfs_context)ctx;

    dev->flowCounter++;
}

wfs_context wfs_init(int pin)
{
    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        return NULL;
    }

    wfs_context dev =
        (wfs_context)malloc(sizeof(struct _wfs_context));

    if (!dev)
        return NULL;

    // zero out context
    memset((void *)dev, 0, sizeof(struct _wfs_context));

    if ( !(dev->gpio = mraa_gpio_init(pin)) )
    {
        printf("%s: mraa_gpio_init() failed.\n", __FUNCTION__);
        wfs_close(dev);
        return NULL;
    }

    mraa_gpio_dir(dev->gpio, MRAA_GPIO_IN);

    wfs_init_clock(dev);

    return dev;
}

void wfs_close(wfs_context dev)
{
    assert(dev != NULL);

    if (dev->isrInstalled)
        wfs_stop_flow_counter(dev);

    if (dev->gpio)
        mraa_gpio_close(dev->gpio);

    free(dev);
}

void wfs_init_clock(const wfs_context dev)
{
    assert(dev != NULL);

    dev->clock = upm_clock_init();
}

uint32_t wfs_get_millis(const wfs_context dev)
{
    assert(dev != NULL);

    return upm_elapsed_ms(&dev->clock);
}

void wfs_clear_flow_counter(const wfs_context dev)
{
    assert(dev != NULL);

    dev->flowCounter = 0;
}

upm_result_t wfs_start_flow_counter(const wfs_context dev)
{
    assert(dev != NULL);

    wfs_init_clock(dev);

    // install our interrupt handler
    if (mraa_gpio_isr(dev->gpio, MRAA_GPIO_EDGE_RISING,
                      &wfs_flow_isr, dev))
    {
        printf("%s: mraa_gpio_isr() failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    dev->isrInstalled = true;
    return UPM_SUCCESS;
}

void wfs_stop_flow_counter(const wfs_context dev)
{
    assert(dev != NULL);

    // remove the interrupt handler
    mraa_gpio_isr_exit(dev->gpio);

    dev->isrInstalled = false;
}

uint32_t wfs_flow_counter(const wfs_context dev)
{
    assert(dev != NULL);

    return dev->flowCounter;
}

float wfs_flow_rate(const wfs_context dev)
{
    assert(dev != NULL);

    uint32_t millis = wfs_get_millis(dev);
    uint32_t flow = wfs_flow_counter(dev);

    // 7.5 comes from the seeedstudio page, see the confusing datasheet :)
    return ((float)flow * 7.5) / ( ((float)millis / 1000.0) * 60.0);
}
