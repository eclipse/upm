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

#include "vk2828u7.h"
#include "upm_fti.h"

/** 
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_vk2828u7_name[] = "VK2828U7";
const char upm_vk2828u7_description[] = "Serial GPS (providing NMEA data)";
const upm_protocol_t upm_vk2828u7_protocol[] = {UPM_UART, UPM_GPIO};
const upm_sensor_t upm_vk2828u7_category[] = {UPM_STREAM};

// forward declarations
const void* upm_vk2828u7_get_ft(upm_sensor_t sensor_type);
void* upm_vk2828u7_init_name();
void upm_vk2828u7_close(void *dev);
int upm_vk2828u7_read(void *dev, char *buffer, int len);
int upm_vk2828u7_write(void *dev, char *buffer, int len);
bool upm_vk2828u7_data_available(void *dev, unsigned int timeout);

static const upm_sensor_ft ft =
{
  .upm_sensor_init_name = &upm_vk2828u7_init_name,
  .upm_sensor_close = &upm_vk2828u7_close,
};

static const upm_stream_ft sft =
{
  .upm_stream_read = upm_vk2828u7_read,
  .upm_stream_write = upm_vk2828u7_write,
  .upm_stream_data_available = upm_vk2828u7_data_available
};

const void* upm_vk2828u7_get_ft(upm_sensor_t sensor_type)
{
  switch(sensor_type)
    {
    case UPM_SENSOR:
      return &ft;
      
    case UPM_STREAM:
      return &sft;
      
    default:
      return NULL;
    }
}

void* upm_vk2828u7_init_name()
{
  return NULL;
}

void upm_vk2828u7_close(void *dev)
{
  vk2828u7_close((vk2828u7_context)dev);
}

int upm_vk2828u7_read(void *dev, char *buffer, int len)
{
  return vk2828u7_read((vk2828u7_context)dev, buffer, len);
}

int upm_vk2828u7_write(void *dev, char *buffer, int len)
{
  return vk2828u7_write((vk2828u7_context)dev, buffer, len);
}

bool upm_vk2828u7_data_available(void *dev, unsigned int timeout)
{
  return vk2828u7_data_available((vk2828u7_context)dev, timeout);
}
