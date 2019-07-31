/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
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
