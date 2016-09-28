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
#include "hka5.h"

#define HKA5_BAUDRATE 9600 // Only baud supported
#define HKA5_PKT_SIZE 32
#define HKA5_PKT_START1 0x42
#define HKA5_PKT_START2 0x4d

static bool verify_cksum(uint8_t *pkt)
{
  uint16_t pkt_cksum = (pkt[30] << 8) | pkt[31];

  int i;
  uint16_t cksum = 0;
  for (i=0; i<(HKA5_PKT_SIZE - 2); i++)
    cksum += pkt[i];

  if (pkt_cksum == cksum)
    return true; // all good
  else
    return false; // :(
}

hka5_context hka5_init(unsigned int uart, int set_pin, int reset_pin)
{
  // make sure MRAA is initialized
  int mraa_rv;
  if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
  {
      printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
      return NULL;
  }

  hka5_context dev =
    (hka5_context)malloc(sizeof(struct _hka5_context));

  if (!dev)
    return NULL;

  // zero out context
  memset((void *)dev, 0, sizeof(struct _hka5_context));

  dev->uart = NULL;
  dev->gpio_reset = NULL;
  dev->gpio_set = NULL;

  // initialize the MRAA contexts

  // uart, default should be 8N1
  if (!(dev->uart = mraa_uart_init(uart)))
    {
      printf("%s: mraa_uart_init() failed.\n", __FUNCTION__);
      hka5_close(dev);
      return NULL;
    }

  if (mraa_uart_set_baudrate(dev->uart, HKA5_BAUDRATE))
    {
      printf("%s: mraa_uart_set_baudrate(%d) failed.\n", __FUNCTION__,
             HKA5_BAUDRATE);
      hka5_close(dev);
      return NULL;
    }

  mraa_uart_set_flowcontrol(dev->uart, false, false);

  // now the set_pin, if enabled
  if (set_pin >= 0)
    {
      if (!(dev->gpio_set = mraa_gpio_init(set_pin)))
        {
          printf("%s: mraa_gpio_init(set) failed.\n", __FUNCTION__);
          hka5_close(dev);
          return NULL;
        }

      mraa_gpio_dir(dev->gpio_set, MRAA_GPIO_OUT);

      // wake up
      hka5_enable(dev, true);
    }

  // now the reset_pin, if enabled
  if (set_pin >= 0)
    {
      if (!(dev->gpio_reset = mraa_gpio_init(reset_pin)))
        {
          printf("%s: mraa_gpio_init(reset) failed.\n", __FUNCTION__);
          hka5_close(dev);
          return NULL;
        }

      mraa_gpio_dir(dev->gpio_reset, MRAA_GPIO_OUT);

      // reset
      hka5_reset(dev);
    }
  return dev;
}

void hka5_close(hka5_context dev)
{
  assert(dev != NULL);

  // sleep
  hka5_enable(dev, false);

  if (dev->uart)
    mraa_uart_stop(dev->uart);
  if (dev->gpio_set)
    mraa_gpio_close(dev->gpio_set);
  if (dev->gpio_reset)
    mraa_gpio_close(dev->gpio_reset);

  free(dev);
}

upm_result_t hka5_enable(const hka5_context dev, bool enable)
{
  assert(dev != NULL);

  if (!dev->gpio_set)
    return UPM_ERROR_NO_RESOURCES;

  if (enable)
    mraa_gpio_write(dev->gpio_set, 1);
  else
    mraa_gpio_write(dev->gpio_set, 0);

  return UPM_SUCCESS;
}

upm_result_t hka5_reset(const hka5_context dev)
{
  assert(dev != NULL);

  if (!dev->gpio_reset)
    return UPM_ERROR_NO_RESOURCES;

  mraa_gpio_write(dev->gpio_set, 0);
  upm_delay(1);
  mraa_gpio_write(dev->gpio_set, 1);
  upm_delay(1);

return UPM_SUCCESS;
}

upm_result_t hka5_update(const hka5_context dev)
{
  assert(dev != NULL);

  uint8_t pkt[HKA5_PKT_SIZE];
  int idx = 0;
  bool done = false;
  char byte;

  memset((void *)pkt, 0, HKA5_PKT_SIZE);

  while (!done)
    {
      // wait up to 2 seconds for data and start
      if (mraa_uart_data_available(dev->uart, 2000))
        {
          while ((mraa_uart_read(dev->uart, &byte, 1) == 1) &&
                 idx < HKA5_PKT_SIZE)
            {
              // first look for a byte starting with HKA5_PKT_START1
              if (idx == 0 && byte != HKA5_PKT_START1)
                continue;

              // look for second byte, make sure it matches
              // HKA5_PKT_START1.  Start over if it's not found.
              if (idx == 1 && byte != HKA5_PKT_START2)
                {
                  // start over
                  idx = 0;
                  continue;
                }

              // else we found the byte, store it and start reading
              // the rest
              pkt[idx++] = (uint8_t)byte;
            }

          if (idx == HKA5_PKT_SIZE)
            {
              done = true;
            }
          else
            {
              // error
              printf("%s: read failed.\n", __FUNCTION__);
              return UPM_ERROR_OPERATION_FAILED;
            }
        }
      else
        {
          printf("%s: read timed out.\n", __FUNCTION__);
          return UPM_ERROR_TIMED_OUT;
        }
    }

  // we have our data
  if (verify_cksum(pkt))
    {
      dev->pm1   = (pkt[4] << 8) | pkt[5];
      dev->pm2_5 = (pkt[6] << 8) | pkt[7];
      dev->pm10  = (pkt[8] << 8) | pkt[9];
    }
  else
    {
      printf("%s: checksum failure.\n", __FUNCTION__);
      // could fail here, but occasional cksum failures are not unusual...
    }

  return UPM_SUCCESS;
}

unsigned int hka5_get_pm1(const hka5_context dev)
{
  assert(dev != NULL);

  return (unsigned int)dev->pm1;
}

unsigned int hka5_get_pm2_5(const hka5_context dev)
{
  assert(dev != NULL);

  return (unsigned int)dev->pm2_5;
}

unsigned int hka5_get_pm10(const hka5_context dev)
{
  assert(dev != NULL);

  return (unsigned int)dev->pm10;
}
