/*
 * Author: Alex Tereschenko <alext.mkrs@gmail.com>
 * Copyright (c) 2018 Alex Tereschenko.
 *
 * Based on LIS2DS12 module by
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * The MIT License
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

#include "lis3dh.h"
#include "upm_fti.h"

/**
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_lis3dh_name[] = "LIS3DH";
const char upm_lis3dh_description[] = "Triple Axis Digital Accelerometer";
const upm_protocol_t upm_lis3dh_protocol[] = { UPM_I2C, UPM_SPI, UPM_GPIO };
const upm_sensor_t upm_lis3dh_category[] = { UPM_ACCELEROMETER };

// Forward declarations
const void* upm_lis3dh_get_ft(upm_sensor_t sensor_type);
void* upm_lis3dh_init_name();
void upm_lis3dh_close(void* dev);
upm_result_t upm_lis3dh_get_value(void* dev, float* value, upm_acceleration_u unit);

const upm_sensor_descriptor_t
upm_lis3dh_get_descriptor()
{
    upm_sensor_descriptor_t usd;
    usd.name = upm_lis3dh_name;
    usd.description = upm_lis3dh_description;
    usd.protocol_size = 3;
    usd.protocol = upm_lis3dh_protocol;
    usd.category_size = 1;
    usd.category = upm_lis3dh_category;
    return usd;
}

static const upm_sensor_ft ft = {
    .upm_sensor_init_name = upm_lis3dh_init_name,
    .upm_sensor_close = upm_lis3dh_close,
};

static const upm_acceleration_ft aft = {.upm_acceleration_get_value = upm_lis3dh_get_value };

const void*
upm_lis3dh_get_ft(upm_sensor_t sensor_type)
{
    switch (sensor_type) {
        case UPM_SENSOR:
            return &ft;

        case UPM_ACCELEROMETER:
            return &aft;

        default:
            return NULL;
    }
}

void*
upm_lis3dh_init_name()
{
    return NULL;
}


void
upm_lis3dh_close(void* dev)
{
    lis3dh_close((lis3dh_context) dev);
}

upm_result_t
upm_lis3dh_get_value(void* dev, float* value, upm_acceleration_u unit)
{
    if (lis3dh_update((lis3dh_context) dev)) {
        return UPM_ERROR_OPERATION_FAILED;
    }

    // No conversion facility in place yet, so we don't do anything
    // with units

    float x, y, z;

    lis3dh_get_accelerometer(dev, &x, &y, &z);

    value[0] = x;
    value[1] = y;
    value[2] = z;

    return UPM_SUCCESS;
}
