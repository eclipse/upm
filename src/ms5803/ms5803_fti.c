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

#include "ms5803.h"
#include "upm_fti.h"

/**
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_ms5803_name[] = "MS5803";
const char upm_ms5803_description[] = "MS5803 Pressure and Temperature Sensor";
const upm_protocol_t upm_ms5803_protocol[] = {UPM_SPI, UPM_I2C, UPM_GPIO};
const upm_sensor_t upm_ms5803_category[] = {UPM_TEMPERATURE, UPM_PRESSURE};

// forward declarations
const void* upm_ms5803_get_ft(upm_sensor_t sensor_type);
void* upm_ms5803_init_name();
void upm_ms5803_close(void *dev);
upm_result_t upm_ms5803_get_pressure(void *dev, float *value);
upm_result_t upm_ms5803_get_temperature(void *dev, float *value,
                                        upm_temperature_u unit);

static const upm_sensor_ft ft =
{
  .upm_sensor_init_name = upm_ms5803_init_name,
  .upm_sensor_close = upm_ms5803_close,
};

static const upm_temperature_ft tft =
{
  .upm_temperature_get_value = upm_ms5803_get_temperature,
};

static const upm_pressure_ft pft =
{
  .upm_pressure_get_value = upm_ms5803_get_pressure,
};

const void* upm_ms5803_get_ft(upm_sensor_t sensor_type)
{
  switch(sensor_type)
    {
    case UPM_SENSOR:
      return &ft;
    case UPM_PRESSURE:
      return &pft;
    case UPM_TEMPERATURE:
      return &tft;
    default:
      return NULL;
    }
}

void* upm_ms5803_init_name()
{
  return NULL;
}

void upm_ms5803_close(void *dev)
{
  ms5803_close((ms5803_context)dev);
}

upm_result_t upm_ms5803_get_pressure(void *dev, float *value)
{
  upm_result_t rv;

  if ((rv = ms5803_update((ms5803_context)dev)))
    return rv;

  *value = ms5803_get_pressure((ms5803_context)dev);

  return UPM_SUCCESS;
}

upm_result_t upm_ms5803_get_temperature(void *dev, float *value,
                                        upm_temperature_u unit)
{
  upm_result_t rv;

  if ((rv = ms5803_update((ms5803_context)dev)))
    return rv;

  // always in C
  float temp = ms5803_get_temperature((ms5803_context)dev);

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
