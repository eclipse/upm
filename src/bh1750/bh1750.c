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

#include <stdio.h>
#include <assert.h>

#include "bh1750.h"

bh1750_context bh1750_init(int bus, uint8_t addr, BH1750_OPMODES_T mode)
{
  // make sure MRAA is initialized
  int mraa_rv;
  if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
  {
      printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
      return NULL;
  }

  bh1750_context dev =
    (bh1750_context)malloc(sizeof(struct _bh1750_context));

  if (!dev)
    {
      printf("%s: context allocation failed.\n", __FUNCTION__);

      return NULL;
    }

  dev->bus = bus;
  dev->is_continuous = false;

  // init the i2c context
  if (!(dev->i2c = mraa_i2c_init(dev->bus)))
    {
      printf("%s: mraa_i2c_init failed.\n", __FUNCTION__);
      free(dev);

      return NULL;
    }

  // now check the address...
  if (mraa_i2c_address(dev->i2c, addr) != MRAA_SUCCESS)
    {
      printf("%s: mraa_i2c_address failed.\n", __FUNCTION__);

      bh1750_close(dev);

      return NULL;
    }

  // set the mode
  if (bh1750_set_opmode(dev, mode) != UPM_SUCCESS)
    {
      printf("%s: bh1750_set_mode failed.\n", __FUNCTION__);

      bh1750_close(dev);

      return NULL;
    }

  return dev;
}

void bh1750_close(const bh1750_context dev)
{
  assert(dev != NULL);

  if (dev->i2c)
    mraa_i2c_stop(dev->i2c);

  free(dev);
}

upm_result_t bh1750_get_lux(const bh1750_context dev, float* lux)
{
  assert(dev != NULL);

  // from the datasheet, page 7
  static const float coeff = 1.2;

  uint16_t raw_lux = 0;

  upm_result_t rv;
  if ((rv = bh1750_read_data(dev, &raw_lux)) != UPM_SUCCESS)
    {
      printf("%s: bh1750_read_data failed.\n", __FUNCTION__);

      return rv;
    }

  *lux = (float)raw_lux;
  *lux /= coeff;

  return rv;
}

bool bh1750_power_up(const bh1750_context dev)
{
  assert(dev != NULL);
  
  if (bh1750_send_command(dev, BH1750_CMD_POWER_UP))
    {
      printf("%s: bh1750_send_command failed.\n", __FUNCTION__);
      
      return false;
    }

  return true;
}
  
bool bh1750_power_down(const bh1750_context dev)
{
  assert(dev != NULL);
  
  if (bh1750_send_command(dev, BH1750_CMD_POWER_DOWN))
    {
      printf("%s: bh1750_send_command failed.\n", __FUNCTION__);
      
      return false;
    }

  return true;
}

bool bh1750_reset(const bh1750_context dev)
{
  assert(dev != NULL);
  
  if (bh1750_send_command(dev, BH1750_CMD_RESET))
    {
      printf("%s: bh1750_send_command failed.\n", __FUNCTION__);
      
      return false;
    }

  return true;
}

upm_result_t bh1750_send_command(const bh1750_context dev, uint8_t cmd)
{
  assert(dev != NULL);

  if (mraa_i2c_write_byte(dev->i2c, cmd))
    {
      printf("%s: mraa_i2c_write_byte failed.\n", __FUNCTION__);
      
      return UPM_ERROR_OPERATION_FAILED;
    }

  return UPM_SUCCESS;
}

upm_result_t bh1750_read_data(const bh1750_context dev, uint16_t* data)
{
  assert(dev != NULL);

  // if we are in a non-continuous mode, we need to power up the
  // device and send the measurement mode command we are interested
  // in.  After the measurement has been read, the device will then
  // power down again.

  if (!dev->is_continuous)
    {
      // power up
      if (!bh1750_power_up(dev))
        {
          printf("%s: bh1750_power_up failed.\n", __FUNCTION__);
          
          return UPM_ERROR_OPERATION_FAILED;
        }

      // send the command, and delay appropriately
      if (bh1750_send_command(dev, dev->opmode))
        {
          printf("%s: bh1750_send_command failed.\n", __FUNCTION__);
          
          return UPM_ERROR_OPERATION_FAILED;
        }

      upm_delay_ms(dev->delayms);
    }

  // now get our data...
  const int num_bytes = 2;
  uint8_t bytes[num_bytes];

  int bytes_read = 0;
  if ((bytes_read = mraa_i2c_read(dev->i2c, bytes, num_bytes)) != num_bytes)
    {
      printf("%s: mraa_i2c_read failed.\n", __FUNCTION__);
      
      return UPM_ERROR_NO_DATA;
    }

  // uncompensated, raw data
  *data = (bytes[0] << 8) | bytes[1];

  return UPM_SUCCESS;
}

upm_result_t bh1750_set_opmode(const bh1750_context dev,
                               BH1750_OPMODES_T mode)
{
  assert(dev != NULL);

  switch(mode)
    {
    case BH1750_OPMODE_H1_CONT:
      dev->is_continuous = true;
      dev->delayms = BH1750_MAX_MEAS_TIME_H;
      dev->opmode = BH1750_CMD_CONT_H_RES_MODE1;
      break;

    case BH1750_OPMODE_H2_CONT:
      dev->is_continuous = true;
      dev->delayms = BH1750_MAX_MEAS_TIME_H;
      dev->opmode = BH1750_CMD_CONT_H_RES_MODE2;
      break;

    case BH1750_OPMODE_H1_ONCE:
      dev->is_continuous = false;
      dev->delayms = BH1750_MAX_MEAS_TIME_H;
      dev->opmode = BH1750_CMD_ONETIME_H_RES_MODE1;
      break;

    case BH1750_OPMODE_H2_ONCE:
      dev->is_continuous = false;
      dev->delayms = BH1750_MAX_MEAS_TIME_H;
      dev->opmode = BH1750_CMD_ONETIME_H_RES_MODE2;
      break;

    case BH1750_OPMODE_L_CONT:
      dev->is_continuous = true;
      dev->delayms = BH1750_MAX_MEAS_TIME_L;
      dev->opmode = BH1750_CMD_CONT_L_RES_MODE;
      break;

    case BH1750_OPMODE_L_ONCE:
      dev->is_continuous = false;
      dev->delayms = BH1750_MAX_MEAS_TIME_L;
      dev->opmode = BH1750_CMD_ONETIME_L_RES_MODE;
      break;

    default:
      printf("%s: Invalid mode.\n", __FUNCTION__);
      return UPM_ERROR_INVALID_PARAMETER;
    }

  // If we are in a continuous mode, power on the device and start
  // measuring.
  
  if (dev->is_continuous)
    {
      if (!bh1750_power_up(dev))
        {
          printf("%s: bh1750_power_up failed.\n", __FUNCTION__);
          
          return UPM_ERROR_OPERATION_FAILED;
        }

      if (bh1750_send_command(dev, dev->opmode))
        {
          printf("%s: bh1750_send_command failed.\n", __FUNCTION__);
          
          return UPM_ERROR_OPERATION_FAILED;
        }

      upm_delay_ms(dev->delayms);
    }
  else
    {
      // if we are not in a continuous mode, power the device off
      if (!bh1750_power_down(dev))
        {
          printf("%s: bh1750_power_down failed.\n", __FUNCTION__);
          
          return UPM_ERROR_OPERATION_FAILED;
        }
    }

  return UPM_SUCCESS;
}



