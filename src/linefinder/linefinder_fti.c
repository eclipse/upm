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

#include "linefinder.h"
#include <upm_fti.h>

/**
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_linefinder_name[] = "LINEFINDER";
const char upm_linefinder_description[] = "Line Finder";
const upm_protocol_t upm_linefinder_protocol[] = {UPM_GPIO};
const upm_sensor_t upm_linefinder_category[] = {UPM_BINARY};

// forward declarations
const void* upm_linefinder_get_ft(upm_sensor_t sensor_type);
void* upm_linefinder_init_name();
void upm_linefinder_close(void *dev);
upm_result_t upm_linefinder_black_detected(void *dev, bool *value);

const upm_sensor_descriptor_t upm_linefinder_get_descriptor()
{
    upm_sensor_descriptor_t usd;
    usd.name = upm_linefinder_name;
    usd.description = upm_linefinder_description;
    usd.protocol_size = 1;
    usd.protocol = upm_linefinder_protocol;
    usd.category_size = 1;
    usd.category = upm_linefinder_category;
    return usd;
}

static const upm_sensor_ft ft =
{
  .upm_sensor_init_name = upm_linefinder_init_name,
  .upm_sensor_close = upm_linefinder_close,
};

static const upm_binary_ft bft =
{
  .upm_binary_get_value = upm_linefinder_black_detected,
};

const void* upm_linefinder_get_ft(upm_sensor_t sensor_type)
{
  switch(sensor_type)
    {
    case UPM_SENSOR:
      return &ft;
    case UPM_BINARY:
      return &bft;
    default:
      return NULL;
    }
}

void *upm_linefinder_init_name()
{
  return NULL;
}

void upm_linefinder_close(void *dev)
{
  linefinder_close((linefinder_context)dev);
}

upm_result_t upm_linefinder_black_detected(void *dev, bool *value)
{
  *value = linefinder_black_detected((linefinder_context)dev);

  return UPM_SUCCESS;
}
