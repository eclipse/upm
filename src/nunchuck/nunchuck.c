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
#include <unistd.h>
#include <assert.h>

#include "nunchuck.h"
#include "upm_utilities.h"

// This is not changeable
#define NUNCHUCK_I2C_ADDR    0x52

// static functions for r/w the device.  No user-serviceable parts
// inside.

static upm_result_t nunchuck_write_byte(const nunchuck_context dev,
                                        uint8_t reg, uint8_t byte)
{
    assert(dev != NULL);

    if (mraa_i2c_write_byte_data(dev->i2c, byte, reg))
    {
        printf("%s: mraa_i2c_write_byte_data() failed\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

static int nunchuck_read_bytes(const nunchuck_context dev, uint8_t reg,
                               uint8_t *buffer, int len)
{
    assert(dev != NULL);

    if (!len || !buffer)
        return 0;

    if (mraa_i2c_write_byte(dev->i2c, reg))
    {
        printf("%s: mraa_i2c_write_byte() failed\n", __FUNCTION__);
        return -1;
    }

    return mraa_i2c_read(dev->i2c, buffer, len);
}

// init
nunchuck_context nunchuck_init(int bus)
{
    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        return NULL;
    }

    nunchuck_context dev =
        (nunchuck_context)malloc(sizeof(struct _nunchuck_context));

    if (!dev)
        return NULL;

    memset((void *)dev, 0, sizeof(struct _nunchuck_context));

    // setup our i2c link
    if ( !(dev->i2c = mraa_i2c_init(bus)) )
    {
        printf("%s: mraa_i2c_init() failed\n", __FUNCTION__);
        nunchuck_close(dev);
        return NULL;
    }

    if (mraa_i2c_address(dev->i2c, NUNCHUCK_I2C_ADDR))
    {
        printf("%s: mraa_i2c_address() failed\n", __FUNCTION__);
        nunchuck_close(dev);
        return NULL;
    }

    // sleep for a second to let things settle
    upm_delay(1);

    // disable encryption
    if (nunchuck_write_byte(dev, 0xf0, 0x55)
        || nunchuck_write_byte(dev, 0xfb, 0x00))
    {
        printf("%s: nunchuck_write_byte() failed\n", __FUNCTION__);
        nunchuck_close(dev);
        return NULL;
    }

    return dev;
}

void nunchuck_close(nunchuck_context dev)
{
    assert(dev != NULL);

    if (dev->i2c)
        mraa_i2c_stop(dev->i2c);

    free(dev);
}

upm_result_t nunchuck_update(const nunchuck_context dev)
{
    assert(dev != NULL);

    const int bufsize = 6;
    uint8_t buf[bufsize];
    int rv;

    rv = nunchuck_read_bytes(dev, 0x00, buf, bufsize);

    if (rv != bufsize)
    {
        printf("%s: nunchuck_read_bytes() failed.  Expected %d, got %d.\n",
               __FUNCTION__, bufsize, rv);
        return UPM_ERROR_OPERATION_FAILED;
    }

    // analog stick X
    dev->stickX = buf[0];

    // analog stick Y
    dev->stickY = buf[1];

    // accelerometer X
    dev->accelX = ( (buf[2] << 2) | ((buf[5] & 0x0c) >> 2) );

    // accelerometer Y
    dev->accelY = ( (buf[3] << 2) | ((buf[5] & 0x30) >> 4) );

    // accelerometer Z
    dev->accelZ = ( (buf[4] << 2) | ((buf[5] & 0xc0) >> 6) );

    // buttonC
    if (buf[5] & 0x02)
        dev->buttonC = false;
    else
        dev->buttonC = true;

    // buttonZ
    if (buf[5] & 0x01)
        dev->buttonZ = false;
    else
        dev->buttonZ = true;

    return UPM_SUCCESS;
}

void nunchuck_get_stick(const nunchuck_context dev, int *x, int *y)
{
    assert(dev != NULL);

    if (x)
        *x = dev->stickX;
    if (y)
        *y = dev->stickY;
}

void nunchuck_get_acceleration(const nunchuck_context dev,
                               int *x, int *y, int *z)
{
    assert(dev != NULL);

    if (x)
        *x = dev->accelX;
    if (y)
        *y = dev->accelY;
    if (z)
        *z = dev->accelZ;
}

void nunchuck_get_buttons(const nunchuck_context dev, bool *c, bool *z)
{
    assert(dev != NULL);

    if (c)
        *c = dev->buttonC;
    if (z)
        *z = dev->buttonZ;
}
