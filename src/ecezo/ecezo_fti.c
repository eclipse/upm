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

#include "ecezo.h"
#include "upm_fti.h"

/**
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_ecezo_name[] = "ECEZO";
const char upm_ecezo_description[] = "EC-EZO EC sensor";
const upm_protocol_t upm_ecezo_protocol[] = {UPM_UART, UPM_I2C};
const upm_sensor_t upm_ecezo_category[] = {UPM_EC};

// forward declarations
const void* upm_ecezo_get_ft(upm_sensor_t sensor_type);
void* upm_ecezo_init_name();
void upm_ecezo_close(void *dev);
upm_result_t upm_ecezo_get_value(const void *dev, float *value);

static const upm_sensor_ft ft =
{
  .upm_sensor_init_name = &upm_ecezo_init_name,
  .upm_sensor_close = &upm_ecezo_close,
};

static const upm_ec_ft ecft =
{
  .upm_ec_get_value = upm_ecezo_get_value
};

const void* upm_ecezo_get_ft(upm_sensor_t sensor_type)
{
  switch(sensor_type)
    {
    case UPM_SENSOR:
      return &ft;

    case UPM_EC:
      return &ecft;

    default:
      return NULL;
    }
}

void* upm_ecezo_init_name()
{
  return NULL;
}

void upm_ecezo_close(void *dev)
{
  ecezo_close((ecezo_context)dev);
}

upm_result_t upm_ecezo_get_value(const void *dev, float *value)
{
    upm_result_t rv;

    if ((rv = ecezo_update((ecezo_context)dev)))
        return rv;

    *value = ecezo_get_ec((ecezo_context)dev);
    return UPM_SUCCESS;
}
