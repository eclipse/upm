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

#include "water.h"
#include "upm_fti.h"

/**
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_water_name[] = "water";
const char upm_water_description[] = "Water presence sensor";
const upm_protocol_t upm_water_protocol[] = {UPM_GPIO};
const upm_sensor_t upm_water_category[] = {UPM_BINARY};

// forward declarations
const void* upm_water_get_ft(upm_sensor_t sensor_type);
void* upm_water_init_name();
void upm_water_close(void *dev);
upm_result_t upm_water_is_wet(void *dev, bool *value);

static const upm_sensor_ft ft =
{
  .upm_sensor_init_name = upm_water_init_name,
  .upm_sensor_close = upm_water_close,
};

static const upm_binary_ft bft =
{
  .upm_binary_get_value = upm_water_is_wet,
};

const void* upm_water_get_ft(upm_sensor_t sensor_type)
{
  switch(sensor_type)
    {
    case UPM_SENSOR:
      return &ft;
    case UPM_BINARY:
      return &bft;
    default:
      return NULL;
    }
}

void *upm_water_init_name()
{
  return NULL;
}

void upm_water_close(void *dev)
{
  water_close((water_context)dev);
}

upm_result_t upm_water_is_wet(void *dev, bool *value)
{
  *value = water_is_wet((water_context)dev);

  return UPM_SUCCESS;
}
