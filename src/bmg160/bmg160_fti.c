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

#include "bmg160.h"
#include "upm_fti.h"

/**
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_bmg160_name[] = "BMG160";
const char upm_bmg160_description[] = "Triple Axis Digital Gyroscope";
const upm_protocol_t upm_bmg160_protocol[] = {UPM_I2C, UPM_SPI, UPM_GPIO};
const upm_sensor_t upm_bmg160_category[] = {UPM_GYROSCOPE};

// forward declarations
const void* upm_bmg160_get_ft(upm_sensor_t sensor_type);
void* upm_bmg160_init_name();
void upm_bmg160_close(void *dev);
upm_result_t upm_bmg160_get_value(void *dev, float *value);

const upm_sensor_descriptor_t upm_bmg160_get_descriptor()
{
    upm_sensor_descriptor_t usd;
    usd.name = upm_bmg160_name;
    usd.description = upm_bmg160_description;
    usd.protocol_size = 3;
    usd.protocol = upm_bmg160_protocol;
    usd.category_size = 1;
    usd.category = upm_bmg160_category;
    return usd;
}

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_bmg160_init_name,
    .upm_sensor_close = &upm_bmg160_close,
};

static const upm_gyroscope_ft gft =
{
    .upm_gyroscope_get_value = &upm_bmg160_get_value
};

const void* upm_bmg160_get_ft(upm_sensor_t sensor_type)
{
  switch(sensor_type)
    {
    case UPM_SENSOR:
      return &ft;

    case UPM_GYROSCOPE:
      return &gft;

    default:
      return NULL;
    }
}

void* upm_bmg160_init_name()
{
    return NULL;
}


void upm_bmg160_close(void *dev)
{
    bmg160_close((bmg160_context)dev);
}

upm_result_t upm_bmg160_get_value(void *dev, float *value)
{
    if (bmg160_update((bmg160_context)dev))
        return UPM_ERROR_OPERATION_FAILED;

    float x, y, z;

    bmg160_get_gyroscope(dev, &x, &y, &z);

    value[0] = x;
    value[1] = y;
    value[2] = z;

    return UPM_SUCCESS;
}
