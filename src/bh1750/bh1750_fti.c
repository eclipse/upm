/*
 * Authors: Jon Trulson <jtrulson@ics.com>
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

#include "bh1750.h"

#include "upm_fti.h"

/**
 * This file implements the Function Table Interface (FTI) for this device
 */


// our descriptor data

static const char upm_bh1750_name[] = "BH1750";
static const char upm_bh1750_description[] = "DFRobot Digital Light Sensor";
static const upm_protocol_t upm_bh1750_protocol[] = {UPM_I2C};
static const upm_sensor_t upm_bh1750_category[] = {UPM_LIGHT};

// Forward declarations
void *upm_bh1750_init_name();
void upm_bh1750_close(void* dev);
upm_result_t upm_bh1750_get_lux(const void* dev, float* lux);

static const upm_sensor_descriptor_t usd = {
  .name = upm_bh1750_name,
  .description = upm_bh1750_description,
  .protocol_size = 1,
  .protocol = upm_bh1750_protocol,
  .category_size = 1,
  .category = upm_bh1750_category,
};
  
const upm_sensor_descriptor_t upm_bh1750_get_descriptor()
{
  return usd;
}

// our generic sensor FT
static const upm_sensor_ft generic_ft =
{
  .upm_sensor_init_name = upm_bh1750_init_name,
  .upm_sensor_close = upm_bh1750_close,
  .upm_sensor_get_descriptor = upm_bh1750_get_descriptor,
};

// our generic light FT
static const upm_light_ft light_ft =
{
  .upm_light_get_value = upm_bh1750_get_lux,
};

const void *upm_bh1750_get_ft(upm_sensor_t sensor_type)
{
  switch (sensor_type)
    {
    case UPM_SENSOR:
      return &generic_ft;
      
    case UPM_LIGHT:
      return &light_ft;
      
    default:
      return NULL;
    }
}

void *upm_bh1750_init_name()
{
  // not implemented, for now call the driver init
  printf("init_name not implemented, calling bh1750_init() with defaults\n");
  return bh1750_init(BH1750_DEFAULT_I2C_BUS, BH1750_DEFAULT_I2C_ADDR,
                     BH1750_OPMODE_H2_ONCE);
}

void upm_bh1750_close(void* dev)
{
  bh1750_close((bh1750_context)dev);
}

// sensor specific

upm_result_t upm_bh1750_get_lux(const void* dev, float* lux)
{
  return bh1750_get_lux((bh1750_context)dev, lux);
}





