/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * These modules were rewritten, based on original work by:
 *
 * (original my9221/groveledbar driver)
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * (grovecircularled driver)
 * Author: Jun Kato and Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Contributions: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <assert.h>
#include <string.h>

#include <upm_utilities.h>
#include <upm_platform.h>

#include "my9221.h"

// 12 LED channels per chip (instance)
#define LEDS_PER_INSTANCE (12)

// forward declarations
static void my9221_lock_data(const my9221_context dev);
static void my9221_send_16bit_block(const my9221_context dev, uint16_t data);

my9221_context my9221_init(int dataPin, int clockPin,
                           int instances)
{
    if (instances < 1)
        instances = 1;

    my9221_context dev =
        (my9221_context)malloc(sizeof(struct _my9221_context));

    if (!dev)
        return NULL;

    memset((void *)dev, 0, sizeof(struct _my9221_context));
    dev->gpioClk = NULL;
    dev->gpioData = NULL;

    // make sure MRAA is initialized
    mraa_result_t mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        my9221_close(dev);
        return NULL;
    }

    // MRAA contexts...
    if ( !(dev->gpioClk = mraa_gpio_init(clockPin)) )
    {
        printf("%s: mraa_gpio_init(clk) failed\n",
               __FUNCTION__);
        my9221_close(dev);
        return NULL;
    }

    mraa_gpio_dir(dev->gpioClk, MRAA_GPIO_OUT);


    if ( !(dev->gpioData = mraa_gpio_init(dataPin)) )
    {
        printf("%s: mraa_gpio_init(data) failed\n",
               __FUNCTION__);
        my9221_close(dev);
        return NULL;
    }

    mraa_gpio_dir(dev->gpioData, MRAA_GPIO_OUT);

    my9221_set_low_intensity_value(dev, 0x00);   // full off
    my9221_set_high_intensity_value(dev, 0xff);  // full bright

    dev->commandWord = 0x0000;       // all defaults
    dev->instances = instances;
    dev->max_leds_per_instance = LEDS_PER_INSTANCE;

    if ( !(dev->bitStates =
           malloc(sizeof(uint16_t) * instances * LEDS_PER_INSTANCE) ) )
    {
        printf("%s: bit state allocation failed\n",
               __FUNCTION__);
        my9221_close(dev);
        return NULL;
    }

    my9221_set_auto_refresh(dev, true);
    my9221_clear_all(dev);


    dev->maxLEDS = dev->instances * LEDS_PER_INSTANCE;
    dev->initialized = true;
    return dev;
}

void my9221_close(my9221_context dev)
{
    assert(dev != NULL);

    if (dev->initialized)
    {
        my9221_clear_all(dev);

        if (!dev->autoRefresh)
            my9221_refresh(dev);
    }

    if (dev->bitStates)
        free(dev->bitStates);

    if (dev->gpioClk)
        mraa_gpio_close(dev->gpioClk);
    if (dev->gpioData)
        mraa_gpio_close(dev->gpioData);

    free(dev);
}

void my9221_set_led(const my9221_context dev, int led, bool on)
{
    assert(dev != NULL);

    int maxLed = dev->maxLEDS - 1;

    if (led > maxLed)
        led = maxLed;
    if (led < 0)
        led = 0;

    dev->bitStates[led] = (on) ? dev->highIntensity : dev->lowIntensity;

    if (dev->autoRefresh)
        my9221_refresh(dev);
}

void my9221_set_low_intensity_value(const my9221_context dev,
                                    int intensity)
{
    assert(dev != NULL);

    dev->lowIntensity = (intensity & 0xff);
}

void my9221_set_high_intensity_value(const my9221_context dev,
                                     int intensity)
{
    assert(dev != NULL);

    dev->highIntensity = (intensity & 0xff);
}

void my9221_set_all(const my9221_context dev)
{
    assert(dev != NULL);

    for (unsigned int i=0; i<dev->maxLEDS; i++)
        dev->bitStates[i] = dev->highIntensity;

    if (dev->autoRefresh)
        my9221_refresh(dev);
}

void my9221_clear_all(const my9221_context dev)
{
    assert(dev != NULL);

    for (unsigned int i=0; i<dev->maxLEDS; i++)
        dev->bitStates[i] = dev->lowIntensity;

    if (dev->autoRefresh)
        my9221_refresh(dev);
}

void my9221_refresh(const my9221_context dev)
{
    assert(dev != NULL);

    for (unsigned int i=0; i<dev->maxLEDS; i++)
    {
        if (i % 12 == 0)
        {
            my9221_send_16bit_block(dev, dev->commandWord);
        }
        my9221_send_16bit_block(dev, dev->bitStates[i]);
    }

    my9221_lock_data(dev);
}

void my9221_set_auto_refresh(const my9221_context dev, bool enable)
{
    assert(dev != NULL);

    dev->autoRefresh = enable;
}

int my9221_get_max_leds(const my9221_context dev)
{
    assert(dev != NULL);

    return dev->maxLEDS;
}

static void my9221_lock_data(const my9221_context dev)
{
    assert(dev != NULL);

    mraa_gpio_write(dev->gpioData, 0);
    upm_delay_us(220);

    for (int idx = 0; idx < 4; idx++)
    {
        mraa_gpio_write(dev->gpioData, 1);
        mraa_gpio_write(dev->gpioData, 0);
    }

    // in reality, we only need > 200ns + (dev->instances * 10ns), so the
    // following should be good for up to dev->instances < 80), if the
    // datasheet is to be believed :)
    upm_delay_us(1);

    return;
}

static void my9221_send_16bit_block(const my9221_context dev, uint16_t data)
{
    assert(dev != NULL);

    for (uint8_t bit_idx = 0; bit_idx < 16; bit_idx++)
    {
        uint32_t state = (data & 0x8000) ? 1 : 0;
        mraa_gpio_write(dev->gpioData, state);

        state = mraa_gpio_read(dev->gpioClk);

        if (state)
            state = 0;
        else
            state = 1;

        mraa_gpio_write(dev->gpioClk, state);

        data <<= 1;
    }

    return;
}
