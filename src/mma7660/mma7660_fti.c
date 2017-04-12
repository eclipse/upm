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

#include "mma7660.h"
#include "upm_fti.h"

/**
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_mma7660_name[] = "MMA7660";
const char upm_mma7660_description[] = "Digital 3-axis Accelerometer";
const upm_protocol_t upm_mma7660_protocol[] = {UPM_I2C, UPM_GPIO};
const upm_sensor_t upm_mma7660_category[] = {UPM_ACCELEROMETER};

// forward declarations
const void* upm_mma7660_get_ft(upm_sensor_t sensor_type);
void* upm_mma7660_init_name();
void upm_mma7660_close(void *dev);
upm_result_t upm_mma7660_get_acceleration(void *dev, float *value,
                                          upm_acceleration_u unit);

const upm_sensor_descriptor_t upm_mma7660_get_descriptor()
{
    upm_sensor_descriptor_t usd;
    usd.name = upm_mma7660_name;
    usd.description = upm_mma7660_description;
    usd.protocol_size = 2;
    usd.protocol = upm_mma7660_protocol;
    usd.category_size = 1;
    usd.category = upm_mma7660_category;
    return usd;
}

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_mma7660_init_name,
    .upm_sensor_close = &upm_mma7660_close,
};

static const upm_acceleration_ft aft =
{
    .upm_acceleration_get_value = &upm_mma7660_get_acceleration
};

const void* upm_mma7660_get_ft(upm_sensor_t sensor_type)
{
  switch(sensor_type)
    {
    case UPM_SENSOR:
      return &ft;

    case UPM_ACCELEROMETER:
      return &aft;

    default:
      return NULL;
    }
}

void* upm_mma7660_init_name()
{
    return NULL;
}


void upm_mma7660_close(void *dev)
{
    mma7660_close((mma7660_context)dev);
}

upm_result_t upm_mma7660_get_acceleration(void *dev, float *value,
                                          upm_acceleration_u unit)
{
  return mma7660_get_acceleration((mma7660_context)dev, &value[0], &value[1],
                                  &value[2]);
}
