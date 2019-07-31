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

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <upm_utilities.h>

#include "mb704x.h"

// The delay to wait (in ms) for a ranging command to complete before
// requesting the range data.
#define MB704X_RANGE_DELAY     (100)

// range command
#define MB704X_CMD_RANGE       (81)

mb704x_context mb704x_init(unsigned int bus, int addr)
{
    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        return NULL;
    }

    mb704x_context dev =
        (mb704x_context)malloc(sizeof(struct _mb704x_context));

    if (!dev)
        return NULL;

    // zero out context
    memset((void *)dev, 0, sizeof(struct _mb704x_context));

    // initialize the MRAA contexts

    if (!(dev->i2c = mraa_i2c_init(bus)))
    {
        printf("%s: mraa_i2c_init() failed.\n", __FUNCTION__);
        mb704x_close(dev);
        return NULL;
    }

    if (mraa_i2c_address(dev->i2c, (uint8_t)addr))
    {
        printf("%s: mraa_i2c_address() failed.\n", __FUNCTION__);
        mb704x_close(dev);
        return NULL;
    }

    // max power up delay
    upm_delay_ms(135);

    return dev;
}

void mb704x_close(mb704x_context dev)
{
    assert(dev != NULL);

    if (dev->i2c)
        mraa_i2c_stop(dev->i2c);

    free(dev);
}

int mb704x_get_range(const mb704x_context dev)
{
    assert(dev != NULL);

    // first, send the range command.
    uint8_t cmd = MB704X_CMD_RANGE;
    if (mraa_i2c_write(dev->i2c, &cmd, 1))
    {
        printf("%s: mraa_i2c_write() failed.\n", __FUNCTION__);
        return -1;
    }

    // delay for measurement
    upm_delay_ms(MB704X_RANGE_DELAY);

    // At this point, we should be able to do our read of the data
    uint8_t buffer[2];
    if (mraa_i2c_read(dev->i2c, buffer, 2) != 2)
    {
        printf("%s: mraa_i2c_read() failed.\n", __FUNCTION__);
        return -1;
    }

    return ((buffer[0] << 8) | buffer[1]);
}
