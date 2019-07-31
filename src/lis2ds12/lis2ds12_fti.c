/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * The MIT License
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include "lis2ds12.h"
#include "upm_fti.h"

/**
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_lis2ds12_name[] = "LIS2DS12";
const char upm_lis2ds12_description[] = "Triple Axis Digital Accelerometer";
const upm_protocol_t upm_lis2ds12_protocol[] = {UPM_I2C, UPM_SPI, UPM_GPIO};
const upm_sensor_t upm_lis2ds12_category[] = {UPM_ACCELEROMETER};

// forward declarations
const void *upm_lis2ds12_get_ft(upm_sensor_t sensor_type);
void *upm_lis2ds12_init_name();
void upm_lis2ds12_close(void *dev);
upm_result_t upm_lis2ds12_get_value(void *dev, float *value,
                                    upm_acceleration_u unit);

const upm_sensor_descriptor_t upm_lis2ds12_get_descriptor()
{
    upm_sensor_descriptor_t usd;
    usd.name = upm_lis2ds12_name;
    usd.description = upm_lis2ds12_description;
    usd.protocol_size = 3;
    usd.protocol = upm_lis2ds12_protocol;
    usd.category_size = 1;
    usd.category = upm_lis2ds12_category;
    return usd;
}

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = upm_lis2ds12_init_name,
    .upm_sensor_close = upm_lis2ds12_close,
};

static const upm_acceleration_ft aft =
{
    .upm_acceleration_get_value = upm_lis2ds12_get_value
};

const void *upm_lis2ds12_get_ft(upm_sensor_t sensor_type)
{
    switch(sensor_type)
    {
    case UPM_SENSOR:
        return &ft;

    case UPM_ACCELEROMETER:
        return &aft;

    default:
        return NULL;
    }
}

void *upm_lis2ds12_init_name()
{
    return NULL;
}


void upm_lis2ds12_close(void *dev)
{
    lis2ds12_close((lis2ds12_context)dev);
}

upm_result_t upm_lis2ds12_get_value(void *dev, float *value,
                                    upm_acceleration_u unit)
{
    if (lis2ds12_update((lis2ds12_context)dev))
        return UPM_ERROR_OPERATION_FAILED;

    // no conversion facility in place yet, so we don't do anything
    // with units

    float x, y, z;

    lis2ds12_get_accelerometer(dev, &x, &y, &z);

    value[0] = x;
    value[1] = y;
    value[2] = z;

    return UPM_SUCCESS;
}
