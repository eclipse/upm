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

#include "bmm150.h"
#include "upm_fti.h"

/**
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_bmm150_name[] = "BMM150";
const char upm_bmm150_description[] = "Triple Axis Digital Magnetometer";
const upm_protocol_t upm_bmm150_protocol[] = {UPM_I2C, UPM_SPI, UPM_GPIO};
const upm_sensor_t upm_bmm150_category[] = {UPM_MAGNETOMETER};

// forward declarations
const void* upm_bmm150_get_ft(upm_sensor_t sensor_type);
void* upm_bmm150_init_name();
void upm_bmm150_close(void *dev);
upm_result_t upm_bmm150_get_value(void *dev, float *value);

const upm_sensor_descriptor_t upm_bmm150_get_descriptor()
{
    upm_sensor_descriptor_t usd;
    usd.name = upm_bmm150_name;
    usd.description = upm_bmm150_description;
    usd.protocol_size = 3;
    usd.protocol = upm_bmm150_protocol;
    usd.category_size = 1;
    usd.category = upm_bmm150_category;
    return usd;
}

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_bmm150_init_name,
    .upm_sensor_close = &upm_bmm150_close,
};

static const upm_magnetometer_ft mft =
{
    .upm_magnetometer_get_value = &upm_bmm150_get_value
};

const void* upm_bmm150_get_ft(upm_sensor_t sensor_type)
{
  switch(sensor_type)
    {
    case UPM_SENSOR:
      return &ft;

    case UPM_MAGNETOMETER:
      return &mft;

    default:
      return NULL;
    }
}

void* upm_bmm150_init_name()
{
    return NULL;
}


void upm_bmm150_close(void *dev)
{
    bmm150_close((bmm150_context)dev);
}

upm_result_t upm_bmm150_get_value(void *dev, float *value)
{
    if (bmm150_update((bmm150_context)dev))
        return UPM_ERROR_OPERATION_FAILED;

    float x, y, z;

    bmm150_get_magnetometer(dev, &x, &y, &z);

    value[0] = x;
    value[1] = y;
    value[2] = z;

    return UPM_SUCCESS;
}
