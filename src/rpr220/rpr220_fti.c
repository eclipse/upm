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

#include "rpr220.h"
#include <upm_fti.h>

/**
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_rpr220_name[] = "RPR220";
const char upm_rpr220_description[] = "IR Reflective Sensor";
const upm_protocol_t upm_rpr220_protocol[] = {UPM_GPIO};
const upm_sensor_t upm_rpr220_category[] = {UPM_BINARY};

// forward declarations
const void* upm_rpr220_get_ft(upm_sensor_t sensor_type);
void* upm_rpr220_init_name();
void upm_rpr220_close(void *dev);
upm_result_t upm_rpr220_black_detected(void *dev, bool *value);

const upm_sensor_descriptor_t upm_rpr220_get_descriptor()
{
    upm_sensor_descriptor_t usd;
    usd.name = upm_rpr220_name;
    usd.description = upm_rpr220_description;
    usd.protocol_size = 1;
    usd.protocol = upm_rpr220_protocol;
    usd.category_size = 1;
    usd.category = upm_rpr220_category;
    return usd;
}

static const upm_sensor_ft ft =
{
  .upm_sensor_init_name = upm_rpr220_init_name,
  .upm_sensor_close = upm_rpr220_close,
};

static const upm_binary_ft bft =
{
  .upm_binary_get_value = upm_rpr220_black_detected,
};

const void* upm_rpr220_get_ft(upm_sensor_t sensor_type)
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

void *upm_rpr220_init_name()
{
  return NULL;
}

void upm_rpr220_close(void *dev)
{
  rpr220_close((rpr220_context)dev);
}

upm_result_t upm_rpr220_black_detected(void *dev, bool *value)
{
  *value = rpr220_black_detected((rpr220_context)dev);

  return UPM_SUCCESS;
}
