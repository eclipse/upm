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

#include "upm_utilities.h"
#include "sht1x.h"

sht1x_context sht1x_init(unsigned int clk_pin, unsigned int data_pin)
{
  // make sure MRAA is initialized
  int mraa_rv;
  if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
  {
      printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
      return NULL;
  }

  sht1x_context dev =
    (sht1x_context)malloc(sizeof(struct _sht1x_context));

  if (!dev)
    return NULL;

  // zero out context
  memset((void *)dev, 0, sizeof(struct _sht1x_context));

  dev->gpio_clk = NULL;
  dev->gpio_data = NULL;

  // initialize the MRAA contexts

  // clock
  if (!(dev->gpio_clk = mraa_gpio_init(clk_pin)))
    {
      printf("%s: mraa_gpio_init(clk) failed.\n", __FUNCTION__);
      sht1x_close(dev);
      return NULL;
    }

  mraa_gpio_dir(dev->gpio_clk, MRAA_GPIO_OUT);

  // data
  if (!(dev->gpio_data = mraa_gpio_init(data_pin)))
    {
      printf("%s: mraa_gpio_init(data) failed.\n", __FUNCTION__);
      sht1x_close(dev);
      return NULL;
    }

  mraa_gpio_dir(dev->gpio_data, MRAA_GPIO_OUT);
  mraa_gpio_mode(dev->gpio_data, MRAA_GPIO_PULLUP);

  // max init time
  upm_delay_ms(15);

  // now read the status register to see if we are highres (14b) or not
  uint8_t status;
  if (sht1x_read_status(dev, &status))
    {
      printf("%s: sht1x_read_status() failed.\n", __FUNCTION__);
      sht1x_close(dev);
      return NULL;
    }

  if (status & SHT1X_STATUS_RESOLUTION_LOW)
    dev->hires = false;
  else
    dev->hires = true;

  // setup our coefficients (see the datasheet).  We always assume 5v
  // here.  We also only deal with Celcius.

  // this will set coeff_d1
  sht1x_set_volts(dev, SHT1X_VOLTS_5);

  dev->coeff_c1 = -2.0468;
  dev->coeff_t1 = 0.01;
  if (dev->hires)
    {
      dev->coeff_d2 = 0.01;
      dev->coeff_c2 = 0.0367;
      dev->coeff_c3 = -1.5955e-6;
      dev->coeff_t2 = 0.00008;
    }
  else
    {
      dev->coeff_d2 = 0.04;
      dev->coeff_c2 = 0.5872;
      dev->coeff_c3 = -4.0845e-4;
      dev->coeff_t2 = 0.00128;
    }

  return dev;
}

void sht1x_close(sht1x_context dev)
{
  assert(dev != NULL);

  if (dev->gpio_clk)
    mraa_gpio_close(dev->gpio_clk);
  if (dev->gpio_data)
    mraa_gpio_close(dev->gpio_data);

  free(dev);
}

upm_result_t sht1x_update(const sht1x_context dev)
{
  assert(dev != NULL);

  // byte 3 is the checksum which we currently ignore
  uint8_t byte1, byte2, byte3;

  // first read the temperature
  sht1x_send_command(dev, SHT1X_CMD_MEAS_TEMPERATURE);
  if (sht1x_wait_for_response(dev))
    {
      printf("%s: wait_for_response(temp) failed.\n", __FUNCTION__);
      return UPM_ERROR_OPERATION_FAILED;
    }

  sht1x_read_8bits(dev, &byte1);
  sht1x_read_8bits(dev, &byte2);
  sht1x_read_8bits(dev, &byte3);

  int temp = (byte1 << 8) | byte2;

  // compute temperature
  dev->temperature = dev->coeff_d1 + dev->coeff_d2 * (float)temp;

  // now get humidity
  sht1x_send_command(dev, SHT1X_CMD_MEAS_HUMIDITY);
  if (sht1x_wait_for_response(dev))
    {
      printf("%s: wait_for_response(hum) failed.\n", __FUNCTION__);
      return UPM_ERROR_OPERATION_FAILED;
    }

  sht1x_read_8bits(dev, &byte1);
  sht1x_read_8bits(dev, &byte2);
  sht1x_read_8bits(dev, &byte3);

  temp = (byte1 << 8) | byte2;

  // first we compute a linear humidity reading, then apply temperature
  // compensation
  float linHumidity = dev->coeff_c1 + dev->coeff_c2 * (float)temp
    + dev->coeff_c3 * (float)temp * (float)temp;

  // convert to "true" RH (temperature compensated)
  dev->humidity = (dev->temperature - 25.0) * (dev->coeff_t1 + dev->coeff_t2)
    + linHumidity;

  if (dev->humidity > 99.0)
    dev->humidity = 100.0;

  return UPM_SUCCESS;
}

void sht1x_start_xmit(const sht1x_context dev)
{
  mraa_gpio_dir(dev->gpio_data, MRAA_GPIO_OUT);

  // start sequence
  mraa_gpio_write(dev->gpio_data, 1);
  mraa_gpio_write(dev->gpio_clk, 1);

  mraa_gpio_write(dev->gpio_data, 0);
  mraa_gpio_write(dev->gpio_clk, 0);

  mraa_gpio_write(dev->gpio_clk, 1);
  mraa_gpio_write(dev->gpio_data, 1);

  mraa_gpio_write(dev->gpio_clk, 0);
}

upm_result_t sht1x_write_8bits(const sht1x_context dev, uint8_t byte)
{
  // send the byte
  mraa_gpio_dir(dev->gpio_data, MRAA_GPIO_OUT);

  int i;
  for (i=0; i<8; i++)
    {
      if (byte & 0x80)
        mraa_gpio_write(dev->gpio_data, 1);
      else
        mraa_gpio_write(dev->gpio_data, 0);

      mraa_gpio_write(dev->gpio_clk, 1);
      mraa_gpio_write(dev->gpio_clk, 0);

      byte <<= 1;
    }

  // now wait for the ack response.  After the falling edge of the 8th
  // clock (above), the data line should be pulled low.  Then, after
  // the falling edge of the ninth clock pulse, the data line should
  // go high.  We check the data line after the rising edge of the
  // ninth clock to make sure it went low.
  bool ackError = false;

  mraa_gpio_dir(dev->gpio_data, MRAA_GPIO_IN);

  // start 9th clock
  mraa_gpio_write(dev->gpio_clk, 1);

  // should be low.  If it's high, there is a problem.
  if (mraa_gpio_read(dev->gpio_data))
    ackError = true;

  // finish 9th clock
  mraa_gpio_write(dev->gpio_clk, 0);

  if (ackError)
    {
      printf("%s: didn't receive proper ACK from SHT1X.\n", __FUNCTION__);
      return UPM_ERROR_OPERATION_FAILED;
    }

  return UPM_SUCCESS;
}

upm_result_t sht1x_send_command(const sht1x_context dev, SHT1X_CMD_T cmd)
{
  assert(dev != NULL);

  sht1x_start_xmit(dev);

  // send the command
  return sht1x_write_8bits(dev, (uint8_t)cmd);
}

upm_result_t sht1x_wait_for_response(const sht1x_context dev)
{
  assert(dev != NULL);

  const int maxRetries = 500;
  int r = 0;

  mraa_gpio_dir(dev->gpio_data, MRAA_GPIO_IN);

  // we wait for some time (about .5 seconds, more than enough time)
  // for the data line to be pulled low.
  while (r++ < maxRetries)
    {
      if (!mraa_gpio_read(dev->gpio_data))
        break;

      upm_delay_ms(1);
    }

  if (r >= maxRetries)
    {
      printf("%s: no response to measurement request.\n", __FUNCTION__);
      return UPM_ERROR_OPERATION_FAILED;
    }

  //  printf("%s: retries: %d\n", __FUNCTION__, r);

  return UPM_SUCCESS;
}

void sht1x_read_8bits(const sht1x_context dev, uint8_t *value)
{
  assert(dev != NULL);

  // we need to read a byte, and acknowlege it

  uint8_t byte = 0;

  mraa_gpio_dir(dev->gpio_data, MRAA_GPIO_IN);

  int i;

  for (i=0; i<8; i++)
    {
      mraa_gpio_write(dev->gpio_clk, 1);

      if (mraa_gpio_read(dev->gpio_data))
        byte |= 1;

      // don't shift on the last bit!
      if (i != 7)
        byte <<= 1;

      mraa_gpio_write(dev->gpio_clk, 0);
    }

  *value = byte;

  // send the ack

  mraa_gpio_dir(dev->gpio_data, MRAA_GPIO_OUT);

  // pull data line low
  mraa_gpio_write(dev->gpio_data, 0);
  // cycle the clock
  mraa_gpio_write(dev->gpio_clk, 1);
  mraa_gpio_write(dev->gpio_clk, 0);
  // release data line
  mraa_gpio_write(dev->gpio_data, 1);
}

float sht1x_get_temperature(const sht1x_context dev)
{
  assert(dev != NULL);

  return dev->temperature;
}

float sht1x_get_humidity(const sht1x_context dev)
{
  assert(dev != NULL);

  return dev->humidity;
}

void sht1x_reset(const sht1x_context dev)
{
  assert(dev != NULL);

  sht1x_send_command(dev, SHT1X_CMD_SOFT_RESET);
  upm_delay_ms(20);
}

upm_result_t sht1x_read_status(const sht1x_context dev, uint8_t *status)
{
  assert(dev != NULL);

  upm_result_t rv;

  if ((rv = sht1x_send_command(dev, SHT1X_CMD_READ_STATUS)))
    {
      printf("%s: send_command() failed.\n", __FUNCTION__);
      return rv;
    }

  sht1x_read_8bits(dev, status);
  return UPM_SUCCESS;
}

upm_result_t sht1x_write_status(const sht1x_context dev, uint8_t status)
{
  assert(dev != NULL);

  upm_result_t rv;

  if ((rv = sht1x_send_command(dev, SHT1X_CMD_WRITE_STATUS)))
    {
      printf("%s: send_command() failed.\n", __FUNCTION__);
      return rv;
    }

  return sht1x_write_8bits(dev, status);
}

void sht1x_set_volts(const sht1x_context dev, SHT1X_VOLTS_T volts)
{
  assert(dev != NULL);

  switch (volts)
    {
    case SHT1X_VOLTS_5:   dev->coeff_d1 = -40.1; break;
    case SHT1X_VOLTS_4:   dev->coeff_d1 = -39.8; break;
    case SHT1X_VOLTS_3_5: dev->coeff_d1 = -39.7; break;
    case SHT1X_VOLTS_3:   dev->coeff_d1 = -39.6; break;
    case SHT1X_VOLTS_2_5: dev->coeff_d1 = -39.4; break;
    }
}
