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

#include "sht1x.h"
#include "upm_fti.h"

/**
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_sht1x_name[] = "SHT1X";
const char upm_sht1x_description[] = "SHT1X Temperature and Humidity Sensor";
const upm_protocol_t upm_sht1x_protocol[] = {UPM_GPIO, UPM_GPIO};
const upm_sensor_t upm_sht1x_category[] = {UPM_TEMPERATURE, UPM_HUMIDITY};

// forward declarations
const void* upm_sht1x_get_ft(upm_sensor_t sensor_type);
void* upm_sht1x_init_name();
void upm_sht1x_close(void *dev);
upm_result_t upm_sht1x_get_humidity(void *dev, float *value);
upm_result_t upm_sht1x_get_temperature(void *dev, float *value,
                                       upm_temperature_u unit);

static const upm_sensor_ft ft =
{
  .upm_sensor_init_name = upm_sht1x_init_name,
  .upm_sensor_close = upm_sht1x_close,
};

static const upm_temperature_ft tft =
{
  .upm_temperature_get_value = upm_sht1x_get_temperature,
};

static const upm_humidity_ft hft =
{
  .upm_humidity_get_value = upm_sht1x_get_humidity,
};

const void* upm_sht1x_get_ft(upm_sensor_t sensor_type)
{
  switch(sensor_type)
    {
    case UPM_SENSOR:
      return &ft;
    case UPM_HUMIDITY:
      return &hft;
    case UPM_TEMPERATURE:
      return &tft;
    default:
      return NULL;
    }
}

void* upm_sht1x_init_name()
{
  return NULL;
}

void upm_sht1x_close(void *dev)
{
  sht1x_close((sht1x_context)dev);
}

upm_result_t upm_sht1x_get_humidity(void *dev, float *value)
{
  upm_result_t rv;

  if ((rv = sht1x_update((sht1x_context)dev)))
    return rv;

  *value = sht1x_get_humidity((sht1x_context)dev);

  return UPM_SUCCESS;
}

upm_result_t upm_sht1x_get_temperature(void *dev, float *value,
                                       upm_temperature_u unit)
{
  upm_result_t rv;

  if ((rv = sht1x_update((sht1x_context)dev)))
    return rv;

  // always in C
  float temp = sht1x_get_temperature((sht1x_context)dev);

  switch (unit)
    {
    case CELSIUS:
      *value = temp;
      return UPM_SUCCESS;

    case KELVIN:
      *value = temp + 273.15;
      return UPM_SUCCESS;

    case FAHRENHEIT:
      *value = temp * (9.0/5.0) + 32.0;
      return UPM_SUCCESS;
    }

  return UPM_SUCCESS;
}
