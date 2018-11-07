/*
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

#include "lsm6ds3h.h"
#include "upm_fti.h"

/**
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_lsm6ds3h_name[] = "LSM6DS3H";
const char upm_lsm6ds3h_description[] =
    "Triple Axis Digital Accelerometer and Gyroscope";
const upm_protocol_t upm_lsm6ds3h_protocol[] = {UPM_I2C, UPM_SPI, UPM_GPIO};
const upm_sensor_t upm_lsm6ds3h_category[] = {UPM_ACCELEROMETER,
                                              UPM_GYROSCOPE};

// forward declarations
const void *upm_lsm6ds3h_get_ft(upm_sensor_t sensor_type);
void *upm_lsm6ds3h_init_name();
void upm_lsm6ds3h_close(void *dev);
upm_result_t upm_lsm6ds3h_get_acc_value(void *dev, float *value,
                                    upm_acceleration_u unit);
upm_result_t upm_lsm6ds3h_get_gyr_value(void *dev, float *value);

const upm_sensor_descriptor_t upm_lsm6ds3h_get_descriptor()
{
    upm_sensor_descriptor_t usd;
    usd.name = upm_lsm6ds3h_name;
    usd.description = upm_lsm6ds3h_description;
    usd.protocol_size = 3;
    usd.protocol = upm_lsm6ds3h_protocol;
    usd.category_size = 2;
    usd.category = upm_lsm6ds3h_category;
    return usd;
}

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = upm_lsm6ds3h_init_name,
    .upm_sensor_close = upm_lsm6ds3h_close,
};

static const upm_acceleration_ft aft =
{
    .upm_acceleration_get_value = upm_lsm6ds3h_get_acc_value
};

static const upm_gyroscope_ft gft =
{
    .upm_gyroscope_get_value = upm_lsm6ds3h_get_gyr_value
};

const void *upm_lsm6ds3h_get_ft(upm_sensor_t sensor_type)
{
    switch(sensor_type)
    {
    case UPM_SENSOR:
        return &ft;

    case UPM_ACCELEROMETER:
        return &aft;

    case UPM_GYROSCOPE:
        return &gft;

    default:
        return NULL;
    }
}

void *upm_lsm6ds3h_init_name()
{
    return NULL;
}

void upm_lsm6ds3h_close(void *dev)
{
    lsm6ds3h_close((lsm6ds3h_context)dev);
}

upm_result_t upm_lsm6ds3h_get_acc_value(void *dev, float *value,
                                        upm_acceleration_u unit)
{
    if (lsm6ds3h_update((lsm6ds3h_context)dev))
        return UPM_ERROR_OPERATION_FAILED;

    // no conversion facility in place yet, so we don't do anything
    // with units

    lsm6ds3h_get_accelerometer(dev, &value[0], &value[1], &value[2]);

    return UPM_SUCCESS;
}

upm_result_t upm_lsm6ds3h_get_gyr_value(void *dev, float *value)
{
    if (lsm6ds3h_update((lsm6ds3h_context)dev))
        return UPM_ERROR_OPERATION_FAILED;

    lsm6ds3h_get_gyroscope(dev, &value[0], &value[1], &value[2]);

    return UPM_SUCCESS;
}
