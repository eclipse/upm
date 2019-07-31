/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * Based on UPM C++ drivers originally developed by:
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * The MIT License
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

#include "lcdks.h"

lcdks_context lcdks_init(int rs, int enable,
                         int d0, int d1, int d2, int d3,
                         int keypad, int backlight)
{
    lcdks_context dev =
        (lcdks_context)malloc(sizeof(struct _lcdks_context));

    if (!dev)
        return NULL;

    memset((void *)dev, 0, sizeof(struct _lcdks_context));

    // make sure MRAA is initialized
    if (mraa_init() != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed.\n", __FUNCTION__);
        lcdks_close(dev);
        return NULL;
    }

    // initialize the lcm1602 context
    if (!(dev->lcm1602 = lcm1602_gpio_init(rs, enable, d0, d1, d2, d3, 16, 2)))
    {
        printf("%s: lcm1602_init failed.\n", __FUNCTION__);
        lcdks_close(dev);
        return NULL;
    }

    // analog keypad
    if (!(dev->aio = mraa_aio_init(keypad)))
    {
        printf("%s: mraa_aio_init() failed.\n", __FUNCTION__);
        lcdks_close(dev);

        return NULL;
    }

    // optional backlight control pin
    if (backlight >= 0)
    {
        if (!(dev->gpio = mraa_gpio_init(backlight)))
        {
            printf("%s: mraa_gpio_init() failed.\n", __FUNCTION__);
            lcdks_close(dev);

            return NULL;
        }

        mraa_gpio_dir(dev->gpio, MRAA_GPIO_OUT);
    }

    // just in case...
    lcdks_backlight_on(dev, true);

    return dev;
}

void lcdks_close(lcdks_context dev)
{
    assert(dev != NULL);

    if (dev->gpio)
        mraa_gpio_close(dev->gpio);
    if (dev->aio)
        mraa_aio_close(dev->aio);
    if (dev->lcm1602)
        lcm1602_close(dev->lcm1602);

    free(dev);
}

upm_result_t lcdks_write(const lcdks_context dev, char *buffer,
                         int len)
{
    assert(dev != NULL);

    return lcm1602_write(dev->lcm1602, buffer, len);
}

upm_result_t lcdks_set_cursor(const lcdks_context dev, unsigned int row,
                              unsigned int column)
{
    assert(dev != NULL);

    return lcm1602_set_cursor(dev->lcm1602, row, column);
}

upm_result_t lcdks_clear(const lcdks_context dev)
{
    assert(dev != NULL);

    return lcm1602_clear(dev->lcm1602);
}

upm_result_t lcdks_home(const lcdks_context dev)
{
    assert(dev != NULL);

    return lcm1602_home(dev->lcm1602);
}

upm_result_t lcdks_create_char(const lcdks_context dev,
                                 unsigned int slot,
                                 char *data)
{
    assert(dev != NULL);

    return lcm1602_create_char(dev->lcm1602, slot, data);
}

upm_result_t lcdks_display_on(const lcdks_context dev, bool on)
{
    assert(dev != NULL);

    return lcm1602_display_on(dev->lcm1602, on);
}

upm_result_t lcdks_cursor_on(const lcdks_context dev, bool on)
{
    assert(dev != NULL);

    return lcm1602_cursor_on(dev->lcm1602, on);
}

upm_result_t lcdks_cursor_blink_on(const lcdks_context dev, bool on)
{
    assert(dev != NULL);

    return lcm1602_cursor_blink_on(dev->lcm1602, on);
}

upm_result_t lcdks_backlight_on(const lcdks_context dev, bool on)
{
    assert(dev != NULL);

    if (dev->gpio)
    {
        if (on)
            mraa_gpio_write(dev->gpio, 1);
        else
            mraa_gpio_write(dev->gpio, 0);
    }

    return UPM_SUCCESS;
}

upm_result_t lcdks_scroll_display_left(const lcdks_context dev)
{
    assert(dev != NULL);

    return lcm1602_scroll_display_left(dev->lcm1602);
}

upm_result_t lcdks_scroll_display_right(const lcdks_context dev)
{
    assert(dev != NULL);

    return lcm1602_scroll_display_right(dev->lcm1602);
}

upm_result_t lcdks_entry_left_to_right(const lcdks_context dev, bool on)
{
    assert(dev != NULL);

    return lcm1602_entry_left_to_right(dev->lcm1602, on);
}

upm_result_t lcdks_autoscroll_on(const lcdks_context dev, bool on)
{
    assert(dev != NULL);

    return lcm1602_autoscroll_on(dev->lcm1602, on);
}

float lcdks_get_key_value(const lcdks_context dev)
{
    assert(dev != NULL);

    return mraa_aio_read_float(dev->aio);
}
