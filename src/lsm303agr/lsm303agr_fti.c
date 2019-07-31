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

#include "lsm303agr.h"
#include "upm_fti.h"

/**
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_lsm303agr_name[] = "LSM303AGR";
const char upm_lsm303agr_description[] =
    "3D Accelerometer and 3D Magnetometer";
const upm_protocol_t upm_lsm303agr_protocol[] = {UPM_I2C, UPM_GPIO};
const upm_sensor_t upm_lsm303agr_category[] = {UPM_MAGNETOMETER,
                                            UPM_ACCELEROMETER};

// forward declarations
const void* upm_lsm303agr_get_ft(upm_sensor_t sensor_type);
void* upm_lsm303agr_init_name();
void upm_lsm303agr_close(void *dev);
upm_result_t upm_lsm303agr_get_acc_value(void *dev, float *value,
                                         upm_acceleration_u unit);
upm_result_t upm_lsm303agr_get_mag_value(void *dev, float *value);

const upm_sensor_descriptor_t upm_lsm303agr_get_descriptor()
{
    upm_sensor_descriptor_t usd;
    usd.name = upm_lsm303agr_name;
    usd.description = upm_lsm303agr_description;
    usd.protocol_size = 2;
    usd.protocol = upm_lsm303agr_protocol;
    usd.category_size = 2;
    usd.category = upm_lsm303agr_category;
    return usd;
}

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_lsm303agr_init_name,
    .upm_sensor_close = &upm_lsm303agr_close,
};

static const upm_magnetometer_ft mft =
{
    .upm_magnetometer_get_value = &upm_lsm303agr_get_mag_value
};

static const upm_acceleration_ft aft =
{
    .upm_acceleration_get_value = &upm_lsm303agr_get_acc_value
};

const void* upm_lsm303agr_get_ft(upm_sensor_t sensor_type)
{
  switch(sensor_type)
    {
    case UPM_SENSOR:
      return &ft;

    case UPM_MAGNETOMETER:
      return &mft;

    case UPM_ACCELEROMETER:
      return &aft;

    default:
      return NULL;
    }
}

void* upm_lsm303agr_init_name()
{
    return NULL;
}


void upm_lsm303agr_close(void *dev)
{
    lsm303agr_close((lsm303agr_context)dev);
}

upm_result_t upm_lsm303agr_get_acc_value(void *dev, float *value,
                                      upm_acceleration_u unit)
{
    if (lsm303agr_update((lsm303agr_context)dev))
        return UPM_ERROR_OPERATION_FAILED;

    float x, y, z;

    lsm303agr_get_magnetometer(dev, &x, &y, &z);

    value[0] = x;
    value[1] = y;
    value[2] = z;

    return UPM_SUCCESS;
}

upm_result_t upm_lsm303agr_get_mag_value(void *dev, float *value)
{
    if (lsm303agr_update((lsm303agr_context)dev))
        return UPM_ERROR_OPERATION_FAILED;

    float x, y, z;

    lsm303agr_get_accelerometer(dev, &x, &y, &z);

    value[0] = x;
    value[1] = y;
    value[2] = z;

    return UPM_SUCCESS;
}
