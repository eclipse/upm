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

#include "mb704x.h"
#include "upm_fti.h"

/**
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_mb704x_name[] = "MB704X";
const char upm_mb704x_description[] = "MB704x MaxSonar-WR Ultrasonic Ranger";
const upm_protocol_t upm_mb704x_protocol[] = {UPM_I2C};
const upm_sensor_t upm_mb704x_category[] = {UPM_DISTANCE};

// forward declarations
const void* upm_mb704x_get_ft(upm_sensor_t sensor_type);
void* upm_mb704x_init_name();
void upm_mb704x_close(void *dev);
upm_result_t upm_mb704x_get_distance(void *dev, float *value,
                                     upm_distance_u unit);

const upm_sensor_descriptor_t upm_es08a_get_descriptor()
{
    upm_sensor_descriptor_t usd;

    usd.name = upm_mb704x_name;
    usd.description = upm_mb704x_description;
    usd.protocol_size = 1;
    usd.protocol = upm_mb704x_protocol;
    usd.category_size = 1;
    usd.category = upm_mb704x_category;

    return usd;
}

static const upm_sensor_ft ft =
{
  .upm_sensor_init_name = &upm_mb704x_init_name,
  .upm_sensor_close = &upm_mb704x_close,
};

static const upm_distance_ft dft =
{
  .upm_distance_get_value = upm_mb704x_get_distance
};

const void* upm_mb704x_get_ft(upm_sensor_t sensor_type)
{
  switch(sensor_type)
    {
    case UPM_SENSOR:
      return &ft;

    case UPM_DISTANCE:
      return &dft;

    default:
      return NULL;
    }
}

void *upm_mb704x_init_name()
{
  return NULL;
}

void upm_mb704x_close(void *dev)
{
  mb704x_close((mb704x_context)dev);
}

upm_result_t upm_mb704x_get_distance(void *dev, float *value,
                                     upm_distance_u unit)
{
    int rv;

    if ((rv = mb704x_get_range((mb704x_context)dev)) < 0)
        return UPM_ERROR_OPERATION_FAILED;

    switch(unit)
    {
    case CENTIMETER:
        *value = (float)rv;
    case INCH:
        *value = ((float)rv * 0.393701);
    default:
        return UPM_ERROR_INVALID_PARAMETER;
    }

    return UPM_SUCCESS;
}
