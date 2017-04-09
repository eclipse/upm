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

#include "dfrorp.h"

#include "upm_utilities.h"

#define DFRORP_NUM_SAMPLES 10
// Resistor R2 value in schematic, in KOhms
#define DFRORP_R2 30.0
// Resistor R3 value in schematic, in KOhms
#define DFRORP_R3 75.0

static float average(const dfrorp_context dev, int samples)
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

dfrorp_context dfrorp_init(unsigned int apin, float a_ref)
{
  // make sure MRAA is initialized
  int mraa_rv;
  if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
  {
      printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
      return NULL;
  }

  dfrorp_context dev =
    (dfrorp_context)malloc(sizeof(struct _dfrorp_context));

  if (!dev)
    return NULL;

  // zero out context
  memset((void *)dev, 0, sizeof(struct _dfrorp_context));

  dev->aio = NULL;

  dev->a_ref = a_ref;

  dev->offset = 0.0;
  dev->scale = 1.0;

  // initialize the MRAA context

  if (!(dev->aio = mraa_aio_init(apin)))
    {
      printf("%s: mraa_aio_init() failed.\n", __FUNCTION__);
      dfrorp_close(dev);
      return NULL;
    }

  // set our analog resolution
  dev->a_res = (float)(1 << mraa_aio_get_bit(dev->aio)) - 1;

  return dev;
}

void dfrorp_close(dfrorp_context dev)
{
  assert(dev != NULL);

  if (dev->aio)
    mraa_aio_close(dev->aio);

  free(dev);
}

void dfrorp_set_offset(const dfrorp_context dev, float offset)
{
  assert(dev != NULL);

  dev->offset = offset;
}

void dfrorp_set_scale(const dfrorp_context dev, float scale)
{
  assert(dev != NULL);

  dev->scale = scale;
}

upm_result_t dfrorp_update(const dfrorp_context dev)
{
  assert(dev != NULL);

  float sample = average(dev, DFRORP_NUM_SAMPLES);
  if (sample == -1.0)
    return UPM_ERROR_OPERATION_FAILED;

  dev->normalized = sample / dev->a_res;
  dev->volts = dev->normalized * dev->a_ref;

  // From the DFRobot site
  dev->orp = ( (DFRORP_R2 * dev->a_ref * 1000.0) -
               (DFRORP_R3 * sample * dev->a_ref * 1000.0 / dev->a_res) ) /
    DFRORP_R3 - dev->orp_cal_offset;

  return UPM_SUCCESS;
}

float dfrorp_get_orp(const dfrorp_context dev)
{
  assert(dev != NULL);

  return dev->orp * dev->scale + (dev->offset * dev->scale);
}

float dfrorp_get_volts(const dfrorp_context dev)
{
  assert(dev != NULL);

  return dev->volts;
}

float dfrorp_get_normalized(const dfrorp_context dev)
{
  assert(dev != NULL);

  return dev->normalized;
}

void dfrorp_set_calibration_offset(const dfrorp_context dev, float offset)
{
  assert(dev != NULL);

  dev->orp_cal_offset = offset;
}
