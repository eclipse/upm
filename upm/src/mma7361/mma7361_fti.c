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

#include "mma7361.h"
#include "upm_fti.h"

/** 
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_mma7361_name[] = "MMA7361";
const char upm_mma7361_description[] = "Analog 3-axis Accelerometer";
const upm_protocol_t upm_mma7361_protocol[] = {UPM_ANALOG, UPM_ANALOG, 
                                               UPM_ANALOG, UPM_GPIO, UPM_GPIO,
                                               UPM_GPIO, UPM_GPIO};
const upm_sensor_t upm_mma7361_category[] = {UPM_ACCELEROMETER};

// forward declarations
const void* upm_mma7361_get_ft(upm_sensor_t sensor_type);
void* upm_mma7361_init_name();
void upm_mma7361_close(void *dev);
upm_result_t upm_mma7361_get_acceleration(void *dev, float *value,
                                          upm_acceleration_u unit);
upm_result_t upm_mma7361_set_scale(void *dev, float *scale);
upm_result_t upm_mma7361_set_offset(void *dev, float *offset);

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_mma7361_init_name,
    .upm_sensor_close = &upm_mma7361_close,
};

static const upm_acceleration_ft aft =
{
    .upm_acceleration_get_value = &upm_mma7361_get_acceleration
};

const void* upm_mma7361_get_ft(upm_sensor_t sensor_type)
{
  switch(sensor_type)
    {
    case UPM_SENSOR:
      return &ft;
      
    case UPM_ACCELEROMETER:
      return &aft;
      
    default:
      return NULL;
    }
}

void* upm_mma7361_init_name()
{
    return NULL;
}


void upm_mma7361_close(void *dev)
{
    mma7361_close((mma7361_context)dev);
}

upm_result_t upm_mma7361_get_acceleration(void *dev, float *value,
                                          upm_acceleration_u unit)
{
  upm_result_t rv;
  if ((rv = mma7361_update((mma7361_context)dev)))
    return rv;

  // Which "value" is supposed to be returned here?  Is it an array?
  // Needs docs in the FTI header file for this sensor type.  We'll
  // assume it's array.
  // FIXME/CHECKME

  // Only in G's
  mma7361_get_acceleration((mma7361_context)dev, &value[0], &value[1],
                           &value[2]);

  return UPM_SUCCESS;
}

upm_result_t upm_mma7361_set_scale(void *dev, float *scale)
{
  // FIXME/CHECKME
  // again, which scale?  As it's a pointer, we'll assume it's
  // an array of 3 floats.  FTI needs some docs.

  mma7361_set_scale((mma7361_context)dev, scale[0], scale[1], scale[2]);
  return UPM_SUCCESS;
}

upm_result_t upm_mma7361_set_offset(void *dev, float *offset)
{
  // FIXME/CHECKME
  // As it's a pointer, we'll assume it's an array of 3 floats.  FTI
  // needs some docs.

  mma7361_set_offset((mma7361_context)dev, offset[0], offset[1], offset[2]);
  return UPM_SUCCESS;
}
