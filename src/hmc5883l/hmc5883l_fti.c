/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
