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

#include "lsm6dsl.h"
#include "upm_fti.h"

/**
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_lsm6dsl_name[] = "LSM6DSL";
const char upm_lsm6dsl_description[] =
    "Triple Axis Digital Accelerometer and Gyroscope";
const upm_protocol_t upm_lsm6dsl_protocol[] = {UPM_I2C, UPM_SPI, UPM_GPIO};
const upm_sensor_t upm_lsm6dsl_category[] = {UPM_ACCELEROMETER,
                                             UPM_GYROSCOPE};

// forward declarations
const void *upm_lsm6dsl_get_ft(upm_sensor_t sensor_type);
void *upm_lsm6dsl_init_name();
void upm_lsm6dsl_close(void *dev);
upm_result_t upm_lsm6dsl_get_acc_value(void *dev, float *value,
                                       upm_acceleration_u unit);
upm_result_t upm_lsm6dsl_get_gyr_value(void *dev, float *value);

const upm_sensor_descriptor_t upm_lsm6dsl_get_descriptor()
{
    upm_sensor_descriptor_t usd;
    usd.name = upm_lsm6dsl_name;
    usd.description = upm_lsm6dsl_description;
    usd.protocol_size = 3;
    usd.protocol = upm_lsm6dsl_protocol;
    usd.category_size = 2;
    usd.category = upm_lsm6dsl_category;
    return usd;
}

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = upm_lsm6dsl_init_name,
    .upm_sensor_close = upm_lsm6dsl_close,
};

static const upm_acceleration_ft aft =
{
    .upm_acceleration_get_value = upm_lsm6dsl_get_acc_value
};

static const upm_gyroscope_ft gft =
{
    .upm_gyroscope_get_value = upm_lsm6dsl_get_gyr_value
};

const void *upm_lsm6dsl_get_ft(upm_sensor_t sensor_type)
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

void *upm_lsm6dsl_init_name()
{
    return NULL;
}

void upm_lsm6dsl_close(void *dev)
{
    lsm6dsl_close((lsm6dsl_context)dev);
}

upm_result_t upm_lsm6dsl_get_acc_value(void *dev, float *value,
                                       upm_acceleration_u unit)
{
    if (lsm6dsl_update((lsm6dsl_context)dev))
        return UPM_ERROR_OPERATION_FAILED;

    // no conversion facility in place yet, so we don't do anything
    // with units

    lsm6dsl_get_accelerometer(dev, &value[0], &value[1], &value[2]);

    return UPM_SUCCESS;
}

upm_result_t upm_lsm6dsl_get_gyr_value(void *dev, float *value)
{
    if (lsm6dsl_update((lsm6dsl_context)dev))
        return UPM_ERROR_OPERATION_FAILED;

    lsm6dsl_get_gyroscope(dev, &value[0], &value[1], &value[2]);

    return UPM_SUCCESS;
}
