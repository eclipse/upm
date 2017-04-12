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

#include "bmg160.h"
#include "upm_fti.h"

/**
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_bmg160_name[] = "BMG160";
const char upm_bmg160_description[] = "Triple Axis Digital Gyroscope";
const upm_protocol_t upm_bmg160_protocol[] = {UPM_I2C, UPM_SPI, UPM_GPIO};
const upm_sensor_t upm_bmg160_category[] = {UPM_GYROSCOPE};

// forward declarations
const void* upm_bmg160_get_ft(upm_sensor_t sensor_type);
void* upm_bmg160_init_name();
void upm_bmg160_close(void *dev);
upm_result_t upm_bmg160_get_value(void *dev, float *value);

const upm_sensor_descriptor_t upm_bmg160_get_descriptor()
{
    upm_sensor_descriptor_t usd;
    usd.name = upm_bmg160_name;
    usd.description = upm_bmg160_description;
    usd.protocol_size = 3;
    usd.protocol = upm_bmg160_protocol;
    usd.category_size = 1;
    usd.category = upm_bmg160_category;
    return usd;
}

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_bmg160_init_name,
    .upm_sensor_close = &upm_bmg160_close,
};

static const upm_gyroscope_ft gft =
{
    .upm_gyroscope_get_value = &upm_bmg160_get_value
};

const void* upm_bmg160_get_ft(upm_sensor_t sensor_type)
{
  switch(sensor_type)
    {
    case UPM_SENSOR:
      return &ft;

    case UPM_GYROSCOPE:
      return &gft;

    default:
      return NULL;
    }
}

void* upm_bmg160_init_name()
{
    return NULL;
}


void upm_bmg160_close(void *dev)
{
    bmg160_close((bmg160_context)dev);
}

upm_result_t upm_bmg160_get_value(void *dev, float *value)
{
    if (bmg160_update((bmg160_context)dev))
        return UPM_ERROR_OPERATION_FAILED;

    float x, y, z;

    bmg160_get_gyroscope(dev, &x, &y, &z);

    value[0] = x;
    value[1] = y;
    value[2] = z;

    return UPM_SUCCESS;
}
