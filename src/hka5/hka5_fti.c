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

#include "hka5.h"
#include "upm_fti.h"

/**
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_hka5_name[] = "HKA5";
const char upm_hka5_description[] = "DFRobot Laser PM2.5 Sensor";
const upm_protocol_t upm_hka5_protocol[] = {UPM_UART, UPM_GPIO, UPM_GPIO};
const upm_sensor_t upm_hka5_category[] = {};

// forward declarations
const void* upm_hka5_get_ft(upm_sensor_t sensor_type);
void* upm_hka5_init_name();
void upm_hka5_close(void *dev);

static const upm_sensor_ft ft =
{
  .upm_sensor_init_name = &upm_hka5_init_name,
  .upm_sensor_close = &upm_hka5_close,
};

const void* upm_hka5_get_ft(upm_sensor_t sensor_type)
{
  switch(sensor_type)
    {
    case UPM_SENSOR:
      return &ft;
    default:
      return NULL;
    }
}

void* upm_hka5_init_name()
{
  return NULL;
}

void upm_hka5_close(void *dev)
{
  hka5_close((hka5_context)dev);
}
