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

#include "p9813.h"
#include "upm_fti.h"

/**
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_p9813_name[] = "P9813";
const char upm_p9813_description[] = "P9813/Chainable RGB LEDs";
const upm_protocol_t upm_p9813_protocol[] = {UPM_GPIO};
const upm_sensor_t upm_p9813_category[] = {};

// forward declarations
const void* upm_p9813_get_ft(upm_sensor_t sensor_type);
void* upm_p9813_init_name();
void upm_p9813_close(void *dev);

static const upm_sensor_ft ft =
{
  .upm_sensor_init_name = &upm_p9813_init_name,
  .upm_sensor_close = &upm_p9813_close,
};

const void* upm_p9813_get_ft(upm_sensor_t sensor_type)
{
  switch(sensor_type)
    {
    case UPM_SENSOR:
      return &ft;
    default:
      return NULL;
    }
}

void* upm_p9813_init_name()
{
  return NULL;
}

void upm_p9813_close(void *dev)
{
  p9813_close((p9813_context)dev);
}
