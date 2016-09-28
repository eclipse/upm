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

// For ublox6 and compatible I2C communications (see the u-blox6
// Receiver Description Protocol Specification datasheet for details).

#define UBLOX6_I2C_BYTES_AVAIL_H                0xfd
#define UBLOX6_I2C_BYTES_AVAIL_L                0xfe
#define UBLOX6_I2C_BYTE_STREAM                  0xff

#define UBLOX6_I2C_BYTE_NONE                    0xff // read if no data avail

// static helpers for i2c reading
static uint8_t readReg(const nmea_gps_context dev, uint8_t reg)
{
  assert(dev != NULL);
  assert(dev->i2c != NULL);

  int rv;
  if ((rv = mraa_i2c_read_byte_data(dev->i2c, reg)) < 0)
    printf("%s: mraa_i2c_read_byte_data() failed.\n", __FUNCTION__);

  // This will return 0xff on errors, which is invalid NMEA data anyway.
  return (uint8_t)rv;
}

static int readRegs(const nmea_gps_context dev, uint8_t reg,
                    uint8_t *buffer, int len)
{
  assert(dev != NULL);
  assert(dev->i2c != NULL);

  int rv;
  if ((rv = mraa_i2c_read_bytes_data(dev->i2c, reg, buffer, len)) < 0)
    {
      printf("%s: mraa_i2c_read_bytes_data() failed.\n", __FUNCTION__);
    }

  return rv;
}

// uart init
nmea_gps_context nmea_gps_init(unsigned int uart, unsigned int baudrate,
                               int enable_pin)
{
  // make sure MRAA is initialized
  int mraa_rv;
  if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
  {
      printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
      return NULL;
  }

  nmea_gps_context dev =
    (nmea_gps_context)malloc(sizeof(struct _nmea_gps_context));

  if (!dev)
    return NULL;

  // zero out context
  memset((void *)dev, 0, sizeof(struct _nmea_gps_context));
  
  dev->uart = NULL;
  dev->i2c = NULL;
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

// i2c ublox init
nmea_gps_context nmea_gps_init_ublox_i2c(unsigned int bus, uint8_t addr)
{
  nmea_gps_context dev =
    (nmea_gps_context)malloc(sizeof(struct _nmea_gps_context));

  if (!dev)
    return NULL;

  // zero out context
  memset((void *)dev, 0, sizeof(struct _nmea_gps_context));

  dev->uart = NULL;
  dev->i2c = NULL;
  dev->gpio_en = NULL;

  // initialize the MRAA contexts

  if (!(dev->i2c = mraa_i2c_init(bus)))
    {
      printf("%s: mraa_i2c_init() failed.\n", __FUNCTION__);
      nmea_gps_close(dev);
      return NULL;
    }

  // This device cannot operate at more than 100Khz, so we set that
  // here and bail if it fails.

  if (mraa_i2c_frequency(dev->i2c, MRAA_I2C_STD))
    {
      printf("%s: mraa_i2c_frequency(MRAA_I2C_STD) failed.\n", __FUNCTION__);
      nmea_gps_close(dev);
      return NULL;
    }

  if (mraa_i2c_address(dev->i2c, addr))
    {
      printf("%s: mraa_i2c_address() failed.\n", __FUNCTION__);
      nmea_gps_close(dev);
      return NULL;
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
  if (dev->i2c)
    mraa_i2c_stop(dev->i2c);
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

  // i2c ublox
  if (dev->i2c)
    {
      int rv;
      if ((rv = readRegs(dev, UBLOX6_I2C_BYTE_STREAM, (uint8_t *)buffer,
                         len)) < 0)
        return rv;

      // now we need to go through the bytes returned, and stop
      // counting "real" bytes when we hit any character with the high
      // bit set.  The documentation implies that only a 0xff will be
      // sent when no new data is available, but it seems sometimes
      // the return contains 0xbf and 0xc3 bytes.  So we stop counting
      // as soon as we see any "8 bit" character (which isn't allowed
      // by NMEA anyway).
      int realBytes = 0;
      int i;

      for (i=0; i<rv; i++)
        {
          if (buffer[i] & 0x80)
            break;

          realBytes++;
        }

      return realBytes;
    }

  // uart
  return mraa_uart_read(dev->uart, buffer, len);
}

int nmea_gps_write(const nmea_gps_context dev, char *buffer, size_t len)
{
  assert(dev != NULL);

  if (!dev->uart)
    return UPM_ERROR_NO_RESOURCES;

  return mraa_uart_write(dev->uart, buffer, len);
}

bool nmea_gps_data_available(const nmea_gps_context dev, unsigned int millis)
{
  assert(dev != NULL);

  // i2c ublox
  if (dev->i2c)
    {
      // here millis is ignored
      uint8_t h, l;

      h = readReg(dev, UBLOX6_I2C_BYTES_AVAIL_H);
      l = readReg(dev, UBLOX6_I2C_BYTES_AVAIL_L);

      uint16_t total = (h << 8) | l;
      // 0 means no data available, 0xffff means read errors
      if (total == 0x0000 || total == 0xffff)
        return false;
      else
        return true;
    }

  // uart
  if (mraa_uart_data_available(dev->uart, millis))
    return true;
  else
    return false;
}

upm_result_t nmea_gps_set_baudrate(const nmea_gps_context dev,
                                   unsigned int baudrate)
{
  assert(dev != NULL);

  if (!dev->uart)
    return UPM_ERROR_NO_RESOURCES;

  if (mraa_uart_set_baudrate(dev->uart, baudrate))
    {
      printf("%s: mraa_uart_set_baudrate() failed.\n", __FUNCTION__);
      return UPM_ERROR_OPERATION_FAILED;
    }      

  return UPM_SUCCESS;
}
