/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * The MIT License
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

#include "bma250e.h"
#include "upm_fti.h"

/**
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_bma250e_name[] = "BMA250E";
const char upm_bma250e_description[] = "Triple Axis Digital Accelerometer";
const upm_protocol_t upm_bma250e_protocol[] = {UPM_I2C, UPM_SPI, UPM_GPIO};
const upm_sensor_t upm_bma250e_category[] = {UPM_ACCELEROMETER};

// forward declarations
const void* upm_bma250e_get_ft(upm_sensor_t sensor_type);
void* upm_bma250e_init_name();
void upm_bma250e_close(void *dev);
upm_result_t upm_bma250e_get_value(void *dev, float *value,
                                   upm_acceleration_u unit);

const upm_sensor_descriptor_t upm_bma250e_get_descriptor()
{
    upm_sensor_descriptor_t usd;
    usd.name = upm_bma250e_name;
    usd.description = upm_bma250e_description;
    usd.protocol_size = 3;
    usd.protocol = upm_bma250e_protocol;
    usd.category_size = 1;
    usd.category = upm_bma250e_category;
    return usd;
}

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_bma250e_init_name,
    .upm_sensor_close = &upm_bma250e_close,
};

static const upm_acceleration_ft aft =
{
    .upm_acceleration_get_value = &upm_bma250e_get_value
};

const void* upm_bma250e_get_ft(upm_sensor_t sensor_type)
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

void* upm_bma250e_init_name()
{
    return NULL;
}


void upm_bma250e_close(void *dev)
{
    bma250e_close((bma250e_context)dev);
}

upm_result_t upm_bma250e_get_value(void *dev, float *value,
                                   upm_acceleration_u unit)
{
    if (bma250e_update((bma250e_context)dev))
        return UPM_ERROR_OPERATION_FAILED;

    // no conversion facility in place yet, so we don't do anything
    // with units

    float x, y, z;

    bma250e_get_accelerometer(dev, &x, &y, &z);

    value[0] = x;
    value[1] = y;
    value[2] = z;

    return UPM_SUCCESS;
}
