/*
 * Author: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include "apa102.h"
#include "upm_fti.h"

/**
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_apa102_name[] = "APA102";
const char upm_apa102_description[] = "APA102/Dotstar LED Strip";
const upm_protocol_t upm_apa102_protocol[] = {UPM_SPI, UPM_GPIO};
const upm_sensor_t upm_apa102_category[] = {};

// forward declarations
const void* upm_apa102_get_ft(upm_sensor_t sensor_type);
void* upm_apa102_init_name();
void upm_apa102_close(void *dev);

static const upm_sensor_ft ft =
{
  .upm_sensor_init_name = &upm_apa102_init_name,
  .upm_sensor_close = &upm_apa102_close,
};

const void* upm_apa102_get_ft(upm_sensor_t sensor_type)
{
  switch(sensor_type)
    {
    case UPM_SENSOR:
      return &ft;
    default:
      return NULL;
    }
}

void* upm_apa102_init_name()
{
  return NULL;
}

void upm_apa102_close(void *dev)
{
  apa102_close((apa102_context)dev);
}
