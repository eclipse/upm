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

#include "hmc5883l.h"
#include "upm_fti.h"

/**
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_hmc5883l_name[] = "HMC5883L";
const char upm_hmc5883l_description[] = "HMC5883L Compass";
const upm_protocol_t upm_hmc5883l_protocol[] = {UPM_I2C};
const upm_sensor_t upm_hmc5883l_category[] = {UPM_COMPASS};

// forward declarations
const void* upm_hmc5883l_get_ft(upm_sensor_t sensor_type);
void* upm_hmc5883l_init_name();
void upm_hmc5883l_close(void *dev);
upm_result_t upm_hmc5883l_get_heading(void *dev, float *value);

static const upm_sensor_ft ft =
{
  .upm_sensor_init_name = upm_hmc5883l_init_name,
  .upm_sensor_close = upm_hmc5883l_close,
};

static const upm_compass_ft cft =
{
  .upm_compass_get_value = upm_hmc5883l_get_heading,
};

const void* upm_hmc5883l_get_ft(upm_sensor_t sensor_type)
{
  switch(sensor_type)
    {
    case UPM_SENSOR:
      return &ft;
    case UPM_COMPASS:
      return &cft;
    default:
      return NULL;
    }
}

void* upm_hmc5883l_init_name()
{
  return NULL;
}

void upm_hmc5883l_close(void *dev)
{
  hmc5883l_close((hmc5883l_context)dev);
}

upm_result_t upm_hmc5883l_get_heading(void *dev, float *value)
{
  upm_result_t rv;

  if ((rv = hmc5883l_update((hmc5883l_context)dev)))
    return rv;

  *value = hmc5883l_heading((hmc5883l_context)dev);

  return UPM_SUCCESS;
}
