/*
 * Author: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
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

#include "apa102.h"
#include "upm_fti.h"

/**
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_apa102_name[] = "APA102";
const char upm_apa102_description[] = "APA102/Dotstar LED Strip";
const upm_protocol_t upm_apa102_protocol[] = {UPM_SPI, UPM_GPIO};
const upm_sensor_t upm_apa102_category[] = {};

// forward declarations
const void* upm_apa102_get_ft(upm_sensor_t sensor_type);
void* upm_apa102_init_name();
void upm_apa102_close(void *dev);

static const upm_sensor_ft ft =
{
  .upm_sensor_init_name = &upm_apa102_init_name,
  .upm_sensor_close = &upm_apa102_close,
};

const void* upm_apa102_get_ft(upm_sensor_t sensor_type)
{
  switch(sensor_type)
    {
    case UPM_SENSOR:
      return &ft;
    default:
      return NULL;
    }
}

void* upm_apa102_init_name()
{
  return NULL;
}

void upm_apa102_close(void *dev)
{
  apa102_close((apa102_context)dev);
}
