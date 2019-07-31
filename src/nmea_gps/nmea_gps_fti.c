/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include "nmea_gps.h"
#include "upm_fti.h"

/** 
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_nmea_gps_name[] = "NMEA_GPS";
const char upm_nmea_gps_description[] = "Serial GPS (providing NMEA data)";
const upm_protocol_t upm_nmea_gps_protocol[] = {UPM_UART, UPM_GPIO};
const upm_sensor_t upm_nmea_gps_category[] = {UPM_STREAM};

// forward declarations
const void* upm_nmea_gps_get_ft(upm_sensor_t sensor_type);
void* upm_nmea_gps_init_name();
void upm_nmea_gps_close(void *dev);
int upm_nmea_gps_read(void *dev, char *buffer, int len);
int upm_nmea_gps_write(void *dev, char *buffer, int len);
bool upm_nmea_gps_data_available(void *dev, unsigned int timeout);

static const upm_sensor_ft ft =
{
  .upm_sensor_init_name = &upm_nmea_gps_init_name,
  .upm_sensor_close = &upm_nmea_gps_close,
};

static const upm_stream_ft sft =
{
  .upm_stream_read = upm_nmea_gps_read,
  .upm_stream_write = upm_nmea_gps_write,
  .upm_stream_data_available = upm_nmea_gps_data_available
};

const void* upm_nmea_gps_get_ft(upm_sensor_t sensor_type)
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

void* upm_nmea_gps_init_name()
{
  return NULL;
}

void upm_nmea_gps_close(void *dev)
{
  nmea_gps_close((nmea_gps_context)dev);
}

int upm_nmea_gps_read(void *dev, char *buffer, int len)
{
  return nmea_gps_read((nmea_gps_context)dev, buffer, len);
}

int upm_nmea_gps_write(void *dev, char *buffer, int len)
{
  return nmea_gps_write((nmea_gps_context)dev, buffer, len);
}

bool upm_nmea_gps_data_available(void *dev, unsigned int timeout)
{
  return nmea_gps_data_available((nmea_gps_context)dev, timeout);
}
