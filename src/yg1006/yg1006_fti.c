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

#include "yg1006.h"
#include <upm_fti.h>

/**
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_yg1006_name[] = "YG1006";
const char upm_yg1006_description[] = "Flame sensor";
const upm_protocol_t upm_yg1006_protocol[] = {UPM_GPIO};
const upm_sensor_t upm_yg1006_category[] = {UPM_BINARY};

// forward declarations
const void* upm_yg1006_get_ft(upm_sensor_t sensor_type);
void* upm_yg1006_init_name();
void upm_yg1006_close(void *dev);
upm_result_t upm_yg1006_flame_detected(void *dev, bool *value);

static const upm_sensor_ft ft =
{
  .upm_sensor_init_name = upm_yg1006_init_name,
  .upm_sensor_close = upm_yg1006_close,
};

static const upm_binary_ft bft =
{
  .upm_binary_get_value = upm_yg1006_flame_detected,
};

const void* upm_yg1006_get_ft(upm_sensor_t sensor_type)
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

void *upm_yg1006_init_name()
{
  return NULL;
}

void upm_yg1006_close(void *dev)
{
  yg1006_close((yg1006_context)dev);
}

upm_result_t upm_yg1006_flame_detected(void *dev, bool *value)
{
  *value = yg1006_flame_detected((yg1006_context)dev);

  return UPM_SUCCESS;
}
