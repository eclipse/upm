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

#include "enc03r.h"
#include "upm_fti.h"

/**
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_enc03r_name[] = "ENC03R";
const char upm_enc03r_description[] = "Analog Single Axis Gyroscope";
const upm_protocol_t upm_enc03r_protocol[] = {UPM_ANALOG};
const upm_sensor_t upm_enc03r_category[] = {UPM_GYROSCOPE};

// forward declarations
const void* upm_enc03r_get_ft(upm_sensor_t sensor_type);
void* upm_enc03r_init_name();
void upm_enc03r_close(void *dev);
upm_result_t upm_enc03r_get_value(void *dev, float *value);

const upm_sensor_descriptor_t upm_enc03r_get_descriptor()
{
    upm_sensor_descriptor_t usd;
    usd.name = upm_enc03r_name;
    usd.description = upm_enc03r_description;
    usd.protocol_size = 1;
    usd.protocol = upm_enc03r_protocol;
    usd.category_size = 1;
    usd.category = upm_enc03r_category;
    return usd;
}

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_enc03r_init_name,
    .upm_sensor_close = &upm_enc03r_close,
};

static const upm_gyroscope_ft gft =
{
    .upm_gyroscope_get_value = &upm_enc03r_get_value
};

const void* upm_enc03r_get_ft(upm_sensor_t sensor_type)
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

void* upm_enc03r_init_name()
{
    return NULL;
}


void upm_enc03r_close(void *dev)
{
    enc03r_close((enc03r_context)dev);
}

upm_result_t upm_enc03r_get_value(void *dev, float *value)
{
    if (enc03r_update((enc03r_context)dev))
        return UPM_ERROR_OPERATION_FAILED;

    value[0] = enc03r_angular_velocity((enc03r_context)dev);
    value[1] = 0.0;
    value[2] = 0.0;

    return UPM_SUCCESS;
}
