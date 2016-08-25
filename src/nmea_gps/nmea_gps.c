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

#include "nmea_gps.h"

#include "upm_utilities.h"

nmea_gps_context nmea_gps_init(unsigned int uart, unsigned int baudrate,
                               int enable_pin)
{
  nmea_gps_context dev =
    (nmea_gps_context)malloc(sizeof(struct _nmea_gps_context));

  if (!dev)
    return NULL;

  // zero out context
  memset((void *)dev, 0, sizeof(struct _nmea_gps_context));
  
  dev->uart = NULL;
  dev->gpio_en = NULL;

  // initialize the MRAA contexts

  // uart, default should be 8N1
  if (!(dev->uart = mraa_uart_init(uart)))
    {
      printf("%s: mraa_uart_init() failed.\n", __FUNCTION__);
      nmea_gps_close(dev);
      return NULL;
    }

  if (nmea_gps_set_baudrate(dev, baudrate))
    {
      printf("%s: nmea_gps_set_baudrate() failed.\n", __FUNCTION__);
      nmea_gps_close(dev);
      return NULL;
    }      

  mraa_uart_set_flowcontrol(dev->uart, false, false);

  // now the gpio_en, if enabled
  if (enable_pin >= 0)
    {
      if (!(dev->gpio_en = mraa_gpio_init(enable_pin)))
        {
          printf("%s: mraa_gpio_init() failed.\n", __FUNCTION__);
          nmea_gps_close(dev);
          return NULL;
        }

      mraa_gpio_dir(dev->gpio_en, MRAA_GPIO_OUT);

      // wake up
      nmea_gps_enable(dev, true);
    }

  return dev;
}

void nmea_gps_close(nmea_gps_context dev)
{
  assert(dev != NULL);

  // sleepy-time
  nmea_gps_enable(dev, false);

  if (dev->uart)
    mraa_uart_stop(dev->uart);
  if (dev->gpio_en)
    mraa_gpio_close(dev->gpio_en);

  free(dev);
}

upm_result_t nmea_gps_enable(const nmea_gps_context dev, bool enable)
{
  assert(dev != NULL);

  if (!dev->gpio_en)
    return UPM_ERROR_NO_RESOURCES;

  if (enable)
    mraa_gpio_write(dev->gpio_en, 1);
  else
    mraa_gpio_write(dev->gpio_en, 0);

  return UPM_SUCCESS;
}

int nmea_gps_read(const nmea_gps_context dev, char *buffer, size_t len)
{
  assert(dev != NULL);

  return mraa_uart_read(dev->uart, buffer, len);
}

int nmea_gps_write(const nmea_gps_context dev, char *buffer, size_t len)
{
  assert(dev != NULL);

  return mraa_uart_write(dev->uart, buffer, len);
}

bool nmea_gps_data_available(const nmea_gps_context dev, unsigned int millis)
{
  assert(dev != NULL);

  if (mraa_uart_data_available(dev->uart, millis))
    return true;
  else
    return false;
}

upm_result_t nmea_gps_set_baudrate(const nmea_gps_context dev,
                                   unsigned int baudrate)
{
  assert(dev != NULL);

  if (mraa_uart_set_baudrate(dev->uart, baudrate))
    {
      printf("%s: mraa_uart_set_baudrate() failed.\n", __FUNCTION__);
      return UPM_ERROR_OPERATION_FAILED;
    }      

  return UPM_SUCCESS;
}
