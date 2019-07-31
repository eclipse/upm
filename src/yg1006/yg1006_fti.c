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

#include "yg1006.h"
#include <upm_fti.h>

/**
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_yg1006_name[] = "YG1006";
const char upm_yg1006_description[] = "Flame sensor";
const upm_protocol_t upm_yg1006_protocol[] = {UPM_GPIO};
const upm_sensor_t upm_yg1006_category[] = {UPM_BINARY};

// forward declarations
const void* upm_yg1006_get_ft(upm_sensor_t sensor_type);
void* upm_yg1006_init_name();
void upm_yg1006_close(void *dev);
upm_result_t upm_yg1006_flame_detected(void *dev, bool *value);

static const upm_sensor_ft ft =
{
  .upm_sensor_init_name = upm_yg1006_init_name,
  .upm_sensor_close = upm_yg1006_close,
};

static const upm_binary_ft bft =
{
  .upm_binary_get_value = upm_yg1006_flame_detected,
};

const void* upm_yg1006_get_ft(upm_sensor_t sensor_type)
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

void *upm_yg1006_init_name()
{
  return NULL;
}

void upm_yg1006_close(void *dev)
{
  yg1006_close((yg1006_context)dev);
}

upm_result_t upm_yg1006_flame_detected(void *dev, bool *value)
{
  *value = yg1006_flame_detected((yg1006_context)dev);

  return UPM_SUCCESS;
}
