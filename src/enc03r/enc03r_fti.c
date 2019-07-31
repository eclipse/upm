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

#include "enc03r.h"
#include "upm_fti.h"

/**
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_enc03r_name[] = "ENC03R";
const char upm_enc03r_description[] = "Analog Single Axis Gyroscope";
const upm_protocol_t upm_enc03r_protocol[] = {UPM_ANALOG};
const upm_sensor_t upm_enc03r_category[] = {UPM_GYROSCOPE};

// forward declarations
const void* upm_enc03r_get_ft(upm_sensor_t sensor_type);
void* upm_enc03r_init_name();
void upm_enc03r_close(void *dev);
upm_result_t upm_enc03r_get_value(void *dev, float *value);

const upm_sensor_descriptor_t upm_enc03r_get_descriptor()
{
    upm_sensor_descriptor_t usd;
    usd.name = upm_enc03r_name;
    usd.description = upm_enc03r_description;
    usd.protocol_size = 1;
    usd.protocol = upm_enc03r_protocol;
    usd.category_size = 1;
    usd.category = upm_enc03r_category;
    return usd;
}

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_enc03r_init_name,
    .upm_sensor_close = &upm_enc03r_close,
};

static const upm_gyroscope_ft gft =
{
    .upm_gyroscope_get_value = &upm_enc03r_get_value
};

const void* upm_enc03r_get_ft(upm_sensor_t sensor_type)
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

void* upm_enc03r_init_name()
{
    return NULL;
}


void upm_enc03r_close(void *dev)
{
    enc03r_close((enc03r_context)dev);
}

upm_result_t upm_enc03r_get_value(void *dev, float *value)
{
    if (enc03r_update((enc03r_context)dev))
        return UPM_ERROR_OPERATION_FAILED;

    value[0] = enc03r_angular_velocity((enc03r_context)dev);
    value[1] = 0.0;
    value[2] = 0.0;

    return UPM_SUCCESS;
}
