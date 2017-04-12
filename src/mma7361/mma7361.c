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

#include "mma7361.h"

#include "upm_utilities.h"

// arduino map() function in macro form (Public Domain)
#define MAP(_value, _in_min, _in_max, _out_min, _out_max) \
  ( ((_value) - (_in_min)) * ((_out_max) - (_out_min)) /  \
    ((_in_max) - (_in_min)) + _out_min )

// While the DFR board can be powered at 5.0 to 3.3v, the analog
// outputs are always going to be 3.3v - ie: the outputs are not
// scaled to 5v, so we need to account for that.
// We may need to make this configurable too in the future...
#define MMA_OUTPUT_AREF 3.3

mma7361_context mma7361_init(int x_pin, int y_pin, int z_pin,
                             int selftest_pin, int sleep_pin,
                             int freefall_pin, int range_pin,
                             float a_ref)
{
  // sanity check - at least one axis needs to be enabled, or what's
  // the point?
  if (x_pin < 0 && y_pin < 0 && z_pin < 0)
    {
      printf("%s: At least one axis must be enabled.\n", __FUNCTION__);
      return NULL;
    }

  // make sure MRAA is initialized
  int mraa_rv;
  if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
  {
      printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
      return NULL;
  }

  mma7361_context dev =
    (mma7361_context)malloc(sizeof(struct _mma7361_context));

  if (!dev)
    return NULL;

  // zero out context
  memset((void *)dev, 0, sizeof(struct _mma7361_context));
  
  dev->aio_x = NULL;
  dev->aio_y = NULL;
  dev->aio_z = NULL;

  dev->gpio_selftest = NULL;
  dev->gpio_sleep = NULL;
  dev->gpio_freefall = NULL;
  dev->gpio_range = NULL;

  dev->a_res = 0;
  dev->a_ref = a_ref;
  dev->g_range = 1.5;

  dev->offset_x = dev->offset_y = dev->offset_z = 0.0;
  dev->scale_x = dev->scale_y = dev->scale_z = 1.0;

  dev->accel_x = 0.0;
  dev->accel_y = 0.0;
  dev->accel_z = 0.0;

  dev->volts_x = 0.0;
  dev->volts_y = 0.0;
  dev->volts_z = 0.0;

  dev->normalized_x = 0;
  dev->normalized_y = 0;
  dev->normalized_z = 0;

  // initialize the MRAA contexts (only what we need)

  // analogs
  if (x_pin >= 0)
    {
      if (!(dev->aio_x = mraa_aio_init(x_pin)))
        {
          printf("%s: mraa_aio_init(x) failed.\n", __FUNCTION__);
          mma7361_close(dev);
          return NULL;
        }
      // set our analog resolution
      dev->a_res = (float)(1 << mraa_aio_get_bit(dev->aio_x)) - 1;
    }

  if (y_pin >= 0)
    {
      if (!(dev->aio_y = mraa_aio_init(y_pin)))
        {
          printf("%s: mraa_aio_init(y) failed.\n", __FUNCTION__);
          mma7361_close(dev);
          return NULL;
        }

      // set our analog resolution if not already set
      if (!dev->a_res)
        dev->a_res = (float)(1 << mraa_aio_get_bit(dev->aio_y)) - 1;
    }

  if (z_pin >= 0)
    {
      if (!(dev->aio_z = mraa_aio_init(z_pin)))
        {
          printf("%s: mraa_aio_init(z) failed.\n", __FUNCTION__);
          mma7361_close(dev);
          return NULL;
        }

      // set our analog resolution if not already set
      if (!dev->a_res)
        dev->a_res = (float)(1 << mraa_aio_get_bit(dev->aio_z)) - 1;
    }

  // now the gpios
  if (selftest_pin >= 0)
    {
      if (!(dev->gpio_selftest = mraa_gpio_init(selftest_pin)))
        {
          printf("%s: mraa_gpio_init(selftest) failed.\n", __FUNCTION__);
          mma7361_close(dev);
          return NULL;
        }

      mraa_gpio_dir(dev->gpio_selftest, MRAA_GPIO_OUT);
      mma7361_selftest(dev, false);
    }

  if (sleep_pin >= 0)
    {
      if (!(dev->gpio_sleep = mraa_gpio_init(sleep_pin)))
        {
          printf("%s: mraa_gpio_init(sleep) failed.\n", __FUNCTION__);
          mma7361_close(dev);
          return NULL;
        }

      mraa_gpio_dir(dev->gpio_sleep, MRAA_GPIO_OUT);
      mma7361_sleep(dev, false);
    }

  if (freefall_pin >= 0)
    {
      if (!(dev->gpio_freefall = mraa_gpio_init(freefall_pin)))
        {
          printf("%s: mraa_gpio_init(freefall) failed.\n", __FUNCTION__);
          mma7361_close(dev);
          return NULL;
        }

      mraa_gpio_dir(dev->gpio_freefall, MRAA_GPIO_IN);
    }

  if (range_pin >= 0)
    {
      if (!(dev->gpio_range = mraa_gpio_init(range_pin)))
        {
          printf("%s: mraa_gpio_init(range) failed.\n", __FUNCTION__);
          mma7361_close(dev);
          return NULL;
        }

      mraa_gpio_dir(dev->gpio_range, MRAA_GPIO_OUT);
      mma7361_set_range(dev, false);
    }

  return dev;
}

void mma7361_close(mma7361_context dev)
{
  assert(dev != NULL);

  // analogs
  if (dev->aio_x)
    mraa_aio_close(dev->aio_x);
  if (dev->aio_y)
    mraa_aio_close(dev->aio_y);
  if (dev->aio_z)
    mraa_aio_close(dev->aio_z);

  // gpios
  if (dev->gpio_selftest)
    mraa_gpio_close(dev->gpio_selftest);
  if (dev->gpio_sleep)
    mraa_gpio_close(dev->gpio_sleep);
  if (dev->gpio_freefall)
    mraa_gpio_close(dev->gpio_freefall);
  if (dev->gpio_range)
    mraa_gpio_close(dev->gpio_range);

  free(dev);
}

upm_result_t mma7361_selftest(const mma7361_context dev, bool selftest)
{
  assert(dev != NULL);

  if (!dev->gpio_selftest)
    return UPM_ERROR_NO_RESOURCES;

  if (selftest)
    mraa_gpio_write(dev->gpio_selftest, 1);
  else
    mraa_gpio_write(dev->gpio_selftest, 0);

  return UPM_SUCCESS;
}

upm_result_t mma7361_sleep(const mma7361_context dev, bool sleep)
{
  assert(dev != NULL);

  if (!dev->gpio_sleep)
    return UPM_ERROR_NO_RESOURCES;

  if (sleep)
    mraa_gpio_write(dev->gpio_sleep, 0);
  else
    mraa_gpio_write(dev->gpio_sleep, 1);

  upm_delay_ms(2);

  return UPM_SUCCESS;
}

upm_result_t mma7361_freefall(const mma7361_context dev, bool *freefall)
{
  assert(dev != NULL);

  if (!dev->gpio_freefall)
    return UPM_ERROR_NO_RESOURCES;

  *freefall = mraa_gpio_read(dev->gpio_freefall) ? true : false;

  return UPM_SUCCESS;
}

void mma7361_set_range(const mma7361_context dev, bool range)
{
  assert(dev != NULL);

  // for this one, if there is no actual GPIO available, we will not
  // error out, but still set the internal range.

  if (range)
    dev->g_range = 6.0;
  else
    dev->g_range = 1.5;

  if (dev->gpio_range)
    {
      if (range)
        mraa_gpio_write(dev->gpio_range, 1);
      else
        mraa_gpio_write(dev->gpio_range, 0);
    }
}

void mma7361_set_offset(const mma7361_context dev, float x, float y, float z)
{
  assert(dev != NULL);

  dev->offset_x = x;
  dev->offset_y = y;
  dev->offset_z = z;
}

void mma7361_set_scale(const mma7361_context dev, float x, float y, float z)
{
  assert(dev != NULL);

  dev->scale_x = x;
  dev->scale_y = y;
  dev->scale_z = z;
}

upm_result_t mma7361_update(const mma7361_context dev)
{
  assert(dev != NULL);

  float sample;

  if (dev->aio_x)
    {
      if ((sample = (float)mraa_aio_read(dev->aio_x)) < 0.0)
        {
          printf("%s: mraa_aio_read(x) failed.\n", __FUNCTION__);
          return UPM_ERROR_OPERATION_FAILED;
        }

      dev->normalized_x = sample / dev->a_res;
      dev->volts_x = dev->normalized_x * dev->a_ref;
      dev->accel_x = MAP(dev->volts_x, 0.0, MMA_OUTPUT_AREF,
                         -dev->g_range, dev->g_range);
    }

  if (dev->aio_y)
    {
      if ((sample = (float)mraa_aio_read(dev->aio_y)) < 0.0)
        {
          printf("%s: mraa_aio_read(y) failed.\n", __FUNCTION__);
          return UPM_ERROR_OPERATION_FAILED;
        }

      dev->normalized_y = sample / dev->a_res;
      dev->volts_y = dev->normalized_y * dev->a_ref;
      dev->accel_y = MAP(dev->volts_y, 0.0, MMA_OUTPUT_AREF,
                         -dev->g_range, dev->g_range);
    }

  if (dev->aio_z)
    {
      if ((sample = (float)mraa_aio_read(dev->aio_z)) < 0.0)
        {
          printf("%s: mraa_aio_read(z) failed.\n", __FUNCTION__);
          return UPM_ERROR_OPERATION_FAILED;
        }

      dev->normalized_z = sample / dev->a_res;
      dev->volts_z = dev->normalized_z * dev->a_ref;
      dev->accel_z = MAP(dev->volts_z, 0.0, MMA_OUTPUT_AREF,
                         -dev->g_range, dev->g_range);
    }

  return UPM_SUCCESS;
}

void mma7361_get_acceleration(const mma7361_context dev,
                              float *x, float *y, float *z)
{
  assert(dev != NULL);

  if (x)
    *x = dev->accel_x * dev->scale_x + (dev->offset_x * dev->scale_x);

  if (y)
    *y = dev->accel_y * dev->scale_y + (dev->offset_y * dev->scale_y);

  if (z)
    *z = dev->accel_z * dev->scale_z + (dev->offset_z * dev->scale_z);
}

void mma7361_get_volts(const mma7361_context dev,
                       float *x, float *y, float *z)
{
  assert(dev != NULL);

  if (x)
    *x = dev->volts_x;

  if (y)
    *y = dev->volts_y;

  if (z)
    *z = dev->volts_z;
}

void mma7361_get_normalized(const mma7361_context dev,
                            float *x, float *y, float *z)
{
  assert(dev != NULL);

  if (x)
    *x = dev->normalized_x;

  if (y)
    *y = dev->normalized_y;

  if (z)
    *z = dev->normalized_z;
}

