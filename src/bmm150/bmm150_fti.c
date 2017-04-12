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

#include "bmm150.h"
#include "upm_fti.h"

/**
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_bmm150_name[] = "BMM150";
const char upm_bmm150_description[] = "Triple Axis Digital Magnetometer";
const upm_protocol_t upm_bmm150_protocol[] = {UPM_I2C, UPM_SPI, UPM_GPIO};
const upm_sensor_t upm_bmm150_category[] = {UPM_MAGNETOMETER};

// forward declarations
const void* upm_bmm150_get_ft(upm_sensor_t sensor_type);
void* upm_bmm150_init_name();
void upm_bmm150_close(void *dev);
upm_result_t upm_bmm150_get_value(void *dev, float *value);

const upm_sensor_descriptor_t upm_bmm150_get_descriptor()
{
    upm_sensor_descriptor_t usd;
    usd.name = upm_bmm150_name;
    usd.description = upm_bmm150_description;
    usd.protocol_size = 3;
    usd.protocol = upm_bmm150_protocol;
    usd.category_size = 1;
    usd.category = upm_bmm150_category;
    return usd;
}

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_bmm150_init_name,
    .upm_sensor_close = &upm_bmm150_close,
};

static const upm_magnetometer_ft mft =
{
    .upm_magnetometer_get_value = &upm_bmm150_get_value
};

const void* upm_bmm150_get_ft(upm_sensor_t sensor_type)
{
  switch(sensor_type)
    {
    case UPM_SENSOR:
      return &ft;

    case UPM_MAGNETOMETER:
      return &mft;

    default:
      return NULL;
    }
}

void* upm_bmm150_init_name()
{
    return NULL;
}


void upm_bmm150_close(void *dev)
{
    bmm150_close((bmm150_context)dev);
}

upm_result_t upm_bmm150_get_value(void *dev, float *value)
{
    if (bmm150_update((bmm150_context)dev))
        return UPM_ERROR_OPERATION_FAILED;

    float x, y, z;

    bmm150_get_magnetometer(dev, &x, &y, &z);

    value[0] = x;
    value[1] = y;
    value[2] = z;

    return UPM_SUCCESS;
}
