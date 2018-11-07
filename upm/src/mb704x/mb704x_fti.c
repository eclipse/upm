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

#include "mb704x.h"
#include "upm_fti.h"

/**
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_mb704x_name[] = "MB704X";
const char upm_mb704x_description[] = "MB704x MaxSonar-WR Ultrasonic Ranger";
const upm_protocol_t upm_mb704x_protocol[] = {UPM_I2C};
const upm_sensor_t upm_mb704x_category[] = {UPM_DISTANCE};

// forward declarations
const void* upm_mb704x_get_ft(upm_sensor_t sensor_type);
void* upm_mb704x_init_name();
void upm_mb704x_close(void *dev);
upm_result_t upm_mb704x_get_distance(void *dev, float *value,
                                     upm_distance_u unit);

const upm_sensor_descriptor_t upm_es08a_get_descriptor()
{
    upm_sensor_descriptor_t usd;

    usd.name = upm_mb704x_name;
    usd.description = upm_mb704x_description;
    usd.protocol_size = 1;
    usd.protocol = upm_mb704x_protocol;
    usd.category_size = 1;
    usd.category = upm_mb704x_category;

    return usd;
}

static const upm_sensor_ft ft =
{
  .upm_sensor_init_name = &upm_mb704x_init_name,
  .upm_sensor_close = &upm_mb704x_close,
};

static const upm_distance_ft dft =
{
  .upm_distance_get_value = upm_mb704x_get_distance
};

const void* upm_mb704x_get_ft(upm_sensor_t sensor_type)
{
  switch(sensor_type)
    {
    case UPM_SENSOR:
      return &ft;

    case UPM_DISTANCE:
      return &dft;

    default:
      return NULL;
    }
}

void *upm_mb704x_init_name()
{
  return NULL;
}

void upm_mb704x_close(void *dev)
{
  mb704x_close((mb704x_context)dev);
}

upm_result_t upm_mb704x_get_distance(void *dev, float *value,
                                     upm_distance_u unit)
{
    int rv;

    if ((rv = mb704x_get_range((mb704x_context)dev)) < 0)
        return UPM_ERROR_OPERATION_FAILED;

    switch(unit)
    {
    case CENTIMETER:
        *value = (float)rv;
    case INCH:
        *value = ((float)rv * 0.393701);
    default:
        return UPM_ERROR_INVALID_PARAMETER;
    }

    return UPM_SUCCESS;
}
