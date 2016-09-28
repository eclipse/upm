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

#include <string.h>
#include <assert.h>

#include "dfrec.h"

#include "upm_utilities.h"

#define DFREC_NUM_SAMPLES 10

// taken from the DFRobot example code
#define TEMPERATURE_COEFF (1.0 + 0.0185)

static float average(const dfrec_context dev, int samples)
{
  int sum = 0;

  if (samples < 1)
    samples = 1;

  int i;
  for (i=0; i< samples; i++)
    {
      int j = mraa_aio_read(dev->aio);
      if (j < 0)
        {
          printf("%s: mraa_aio_read() failed.\n", __FUNCTION__);
          return -1.0;
        }
      sum += j;
      upm_delay_ms(20);
    }

  return (float)(sum / samples);
}

dfrec_context dfrec_init(unsigned int apin, unsigned int uart_ow,
                         unsigned int device_idx, float a_ref)
{
  // make sure MRAA is initialized
  int mraa_rv;
  if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
  {
      printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
      return NULL;
  }

  dfrec_context dev =
    (dfrec_context)malloc(sizeof(struct _dfrec_context));

  if (!dev)
    return NULL;

  // zero out context
  memset((void *)dev, 0, sizeof(struct _dfrec_context));

  dev->aio = NULL;
  dev->ds18b20 = NULL;
  dev->device_idx = device_idx;

  dev->a_ref = a_ref;

  dev->offset = 0.0;
  dev->scale = 1.0;

  // init the defaults based on the DFRobot code example
  dev->thres_min = 150;
  dev->thres_max = 3300;

  dev->thres_1 = 448;
  dev->scale_1 = 6.84;
  dev->offset_1 = -64.32;

  dev->thres_2 = 1457;
  dev->scale_2 = 6.98;
  dev->offset_2 = -127;

  dev->scale_3 = 5.3;
  dev->offset_3 = 2278;

  // initialize the AIO context

  if (!(dev->aio = mraa_aio_init(apin)))
    {
      printf("%s: mraa_aio_init() failed.\n", __FUNCTION__);
      dfrec_close(dev);
      return NULL;
    }

  // set our analog resolution
  dev->a_res = (float)(1 << mraa_aio_get_bit(dev->aio)) - 1;

  // inistialize our ds18b20 context
  if (!(dev->ds18b20 = ds18b20_init(uart_ow)))
    {
      printf("%s: ds18b20_init() failed.\n", __FUNCTION__);
      dfrec_close(dev);
      return NULL;
    }

  // make sure the device index exists...
  if (dev->device_idx >= ds18b20_devices_found(dev->ds18b20))
    {
      printf("%s: ds18b20 device index %d does not exist. Max index is %d\n",
             __FUNCTION__, dev->device_idx,
             ds18b20_devices_found(dev->ds18b20) - 1);
      dfrec_close(dev);
      return NULL;
    }

  return dev;
}

void dfrec_close(dfrec_context dev)
{
  assert(dev != NULL);

  if (dev->aio)
    mraa_aio_close(dev->aio);

  if (dev->ds18b20)
    ds18b20_close(dev->ds18b20);

  free(dev);
}

void dfrec_set_offset(const dfrec_context dev, float offset)
{
  assert(dev != NULL);

  dev->offset = offset;
}

void dfrec_set_scale(const dfrec_context dev, float scale)
{
  assert(dev != NULL);

  dev->scale = scale;
}

upm_result_t dfrec_update(const dfrec_context dev)
{
  assert(dev != NULL);

  // update temperature
  ds18b20_update(dev->ds18b20, dev->device_idx);
  dev->temperature = ds18b20_get_temperature(dev->ds18b20, dev->device_idx);

  float sample = average(dev, DFREC_NUM_SAMPLES);

  if (sample == -1.0)
    return UPM_ERROR_OPERATION_FAILED;

  dev->normalized = sample / dev->a_res;
  dev->volts = dev->normalized * dev->a_ref;

  float mVolts = dev->volts * 1000.0;
  float tempCoefficient = TEMPERATURE_COEFF * (dev->temperature - 25.0);
  float voltageCoefficient = mVolts / tempCoefficient;

  if (voltageCoefficient < dev->thres_min)
    {
      printf("%s: Not in solution (voltageCoefficient %f < %f).\n",
             __FUNCTION__, voltageCoefficient, dev->thres_min);
      dev->ec = 0.0;
      return UPM_ERROR_OUT_OF_RANGE;
    }
  else if (voltageCoefficient > dev->thres_max)
    {
      printf("%s: Out of range (voltageCoefficient %f > %f).\n",
             __FUNCTION__, voltageCoefficient, dev->thres_max);
      dev->ec = 0.0;
      return UPM_ERROR_OUT_OF_RANGE;
    }
  else
    {
      if (voltageCoefficient <= dev->thres_1)
        {
          // 1ms/cm<EC<=3ms/cm
          dev->ec = dev->scale_1 * voltageCoefficient + dev->offset_1;
        }
      else if (voltageCoefficient <= dev->thres_2)
        {
          // 3ms/cm<EC<=10ms/cm
          dev->ec = dev->scale_2 * voltageCoefficient + dev->offset_2;
        }
      else
        {
          // 10ms/cm<EC<20ms/cm
          dev->ec = dev->scale_3 * voltageCoefficient + dev->offset_3;
        }

      // convert us/cm to ms/cm
      dev->ec /= 1000.0;
    }

  return UPM_SUCCESS;
}

float dfrec_get_ec(const dfrec_context dev)
{
  assert(dev != NULL);

  return dev->ec * dev->scale + (dev->offset * dev->scale);
}

float dfrec_get_temperature(const dfrec_context dev)
{
  assert(dev != NULL);

  return dev->temperature;
}

float dfrec_get_volts(const dfrec_context dev)
{
  assert(dev != NULL);

  return dev->volts;
}

float dfrec_get_normalized(const dfrec_context dev)
{
  assert(dev != NULL);

  return dev->normalized;
}

void dfrec_set_threshold_min_max(const dfrec_context dev, float min,
                                 float max)
{
  assert(dev != NULL);

  dev->thres_min = min;
  dev->thres_max = max;
}

void dfrec_set_threshold_1(const dfrec_context dev, float thres,
                           float scale, float offset)
{
  assert(dev != NULL);

  dev->thres_1 = thres;
  dev->scale_1 = scale;
  dev->offset_1 = scale;
}

void dfrec_set_threshold_2(const dfrec_context dev, float thres,
                           float scale, float offset)
{
  assert(dev != NULL);

  dev->thres_2 = thres;
  dev->scale_2 = scale;
  dev->offset_2 = scale;
}

void dfrec_set_threshold_3(const dfrec_context dev, float scale, float offset)
{
  assert(dev != NULL);

  dev->scale_3 = scale;
  dev->offset_3 = scale;
}
