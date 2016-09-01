/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
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

#include "dfrorp.h"
#include "upm_fti.h"

/**
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_dfrorp_name[] = "DFRORP";
const char upm_dfrorp_description[] = "DFRobot Analog ORP Sensor";
const upm_protocol_t upm_dfrorp_protocol[] = {UPM_ANALOG};
const upm_sensor_t upm_dfrorp_category[] = {UPM_ORP};

// forward declarations
const void* upm_dfrorp_get_ft(upm_sensor_t sensor_type);
void* upm_dfrorp_init_name();
void upm_dfrorp_close(void *dev);
upm_result_t upm_dfrorp_get_value(const void *dev, float *value);
upm_result_t upm_dfrorp_set_scale(const void *dev, float scale);
upm_result_t upm_dfrorp_set_offset(const void *dev, float offset);

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_dfrorp_init_name,
    .upm_sensor_close = &upm_dfrorp_close,
};

static const upm_orp_ft orpft =
{
    .upm_orp_set_offset = upm_dfrorp_set_offset,
    .upm_orp_set_scale = upm_dfrorp_set_scale,
    .upm_orp_get_value = upm_dfrorp_get_value
};

const void* upm_dfrorp_get_ft(upm_sensor_t sensor_type)
{
  switch(sensor_type)
    {
    case UPM_SENSOR:
      return &ft;

    case UPM_ORP:
      return &orpft;

    default:
      return NULL;
    }
}

void* upm_dfrorp_init_name()
{
    return NULL;
}

void upm_dfrorp_close(void *dev)
{
    dfrorp_close((dfrorp_context)dev);
}

upm_result_t upm_dfrorp_set_scale(const void *dev, float scale)
{
  dfrorp_set_scale((dfrorp_context)dev, scale);
  return UPM_SUCCESS;
}

upm_result_t upm_dfrorp_set_offset(const void *dev, float offset)
{
  dfrorp_set_offset((dfrorp_context)dev, offset);
  return UPM_SUCCESS;
}

upm_result_t upm_dfrorp_get_value(const void *dev, float *value)
{
  *value = dfrorp_get_orp((dfrorp_context)dev);
  return UPM_SUCCESS;
}
