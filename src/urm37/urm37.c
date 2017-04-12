/*
 * Author: Jon Trulson <jtrulson@ics.com>
 *         Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * Thanks to Adafruit for supplying a google translated version of the
 * Chinese datasheet and some clues in their code.
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

#include "urm37.h"

#include "upm_utilities.h"

#define URM37_MAX_DATA_LEN      4
#define URM37_WAIT_TIMEOUT      1000
#define URM37_MAX_RETRIES       10

urm37_context urm37_init(uint8_t a_pin, uint8_t reset_pin,
                         uint8_t trigger_pin, float a_ref,
                         uint8_t uart_bus, bool analog_mode)
{
  // make sure MRAA is initialized
  int mraa_rv;
  if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
  {
      printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
      return NULL;
  }

  urm37_context dev = (urm37_context)malloc(sizeof(struct _urm37_context));

  if (!dev)
    return NULL;

  // clear out context
  memset((void *)dev, 0, sizeof(struct _urm37_context));
  
  // NULL out MRAA contexts for now (redundant with memset I know, but...)
  dev->aio = NULL;
  dev->gpio_reset = NULL;
  dev->gpio_trigger = NULL;
  dev->uart = NULL;

  dev->a_res = 0;
  dev->a_ref = a_ref;

  // set the mode
  dev->is_analog_mode = analog_mode;

  // initialize the MRAA contexts (only what we need)

  // analog only
  if (dev->is_analog_mode)
    {
      if (!(dev->aio = mraa_aio_init(a_pin)))
        {
          printf("%s: mraa_aio_init() failed.\n", __FUNCTION__);
          urm37_close(dev);
          return NULL;
        }

      // ADC resolution
      dev->a_res = (float)(1 << mraa_aio_get_bit(dev->aio)) - 1;

      if (!(dev->gpio_trigger = mraa_gpio_init(trigger_pin)))
        {
          printf("%s: mraa_gpio_init(trigger) failed.\n", __FUNCTION__);
          urm37_close(dev);
          return NULL;
        }

      mraa_gpio_dir(dev->gpio_trigger, MRAA_GPIO_OUT);
      mraa_gpio_write(dev->gpio_trigger, 1);
    }
  else
    {
      // UART only
      if (!(dev->uart = mraa_uart_init(uart_bus)))
        {
          printf("%s: mraa_uart_init() failed.\n", __FUNCTION__);
          urm37_close(dev);
          return NULL;
        }

      mraa_uart_set_baudrate(dev->uart, 9600);
      mraa_uart_set_non_blocking(dev->uart, false);
    }

  // reset - used by both analog and uart modes
  if (!(dev->gpio_reset = mraa_gpio_init(reset_pin)))
    {
      printf("%s: mraa_gpio_init(reset) failed.\n", __FUNCTION__);
      urm37_close(dev);
      return NULL;
    }

  mraa_gpio_dir(dev->gpio_reset, MRAA_GPIO_OUT);

  urm37_reset(dev);

  return dev;
}

void urm37_close(urm37_context dev)
{
  if (dev->aio)
    mraa_aio_close(dev->aio);
  if (dev->gpio_reset)
    mraa_gpio_close(dev->gpio_reset);
  if (dev->gpio_trigger)
    mraa_gpio_close(dev->gpio_trigger);
  if (dev->uart)
    mraa_uart_stop(dev->uart);

  free(dev);
}

upm_result_t urm37_reset(urm37_context dev)
{
  mraa_gpio_write(dev->gpio_reset, 0);
  upm_delay_us(100);
  mraa_gpio_write(dev->gpio_reset, 1);

  // wait for reset to complete
  upm_delay(3);

  return UPM_SUCCESS;
}

// private
static bool urm37_data_available(urm37_context dev, uint32_t millis)
{
  if (mraa_uart_data_available(dev->uart, millis))
    return true;
  else
    return false;
}

// private
static int urm37_read_data(urm37_context dev, char* data)
{
  return mraa_uart_read(dev->uart, data, (size_t)URM37_MAX_DATA_LEN);
}

// private
static int urm37_write_data(urm37_context dev, const char* data)
{
  mraa_uart_flush(dev->uart);
  return mraa_uart_write(dev->uart, data, (size_t)URM37_MAX_DATA_LEN);
}

upm_result_t urm37_send_command(urm37_context dev, char* cmd, char* response)
{
  if (dev->is_analog_mode)
    {
      printf("%s: UART commands are not available in analog mode\n",
             __FUNCTION__);
      
      return UPM_ERROR_NOT_SUPPORTED;
    }

  int tries = 0;

  while (tries++ < URM37_MAX_RETRIES)
    {
      if (urm37_write_data(dev, cmd) < 0)
        {
          // A write error of some kind.  We don't try to continue
          // after this.
          printf("%s: write_data() failed\n", __FUNCTION__);

          return UPM_ERROR_OPERATION_FAILED;
        }

      if (!urm37_data_available(dev, URM37_WAIT_TIMEOUT))
        {
          // timeout, retry...
          continue;
        }

      int rv = urm37_read_data(dev, response);
      if (rv < 0)
        {
          printf("%s: read_data() failed\n", __FUNCTION__);
          return UPM_ERROR_OPERATION_FAILED;
        }

      if (rv != URM37_MAX_DATA_LEN)
        {
          // read wrong number of bytes...
          printf("%s: read_data() returned %d bytes, expected %d, retrying\n",
                 __FUNCTION__, rv, URM37_MAX_DATA_LEN);
          continue;
        }
      else
        {
          // we have data, verify cksum, return the response if it's
          // good, retry otherwise
          uint8_t cksum = (uint8_t)(response[0] + response[1] + response[2]);

          if ((uint8_t)response[3] != cksum)
            {
              printf("%s: checksum failure: got %d, expected %d, retrying\n",
                     __FUNCTION__, (int)response[3], (int)cksum);
              continue;
            }

          // all good
          return UPM_SUCCESS;
        }
    }

  // If we are here, we timed out and all retries were exhausted
  return UPM_ERROR_TIMED_OUT;
}

upm_result_t urm37_get_distance(urm37_context dev, float *distance,
                                int degrees)
{
  if (dev->is_analog_mode)
    {
      // analog mode
      int val;

      // send the trigger pulse and sample
      mraa_gpio_write(dev->gpio_trigger, 0);
      val = mraa_aio_read(dev->aio);
      mraa_gpio_write(dev->gpio_trigger, 1);

      // convert to mV
      float volts = ((float)val * (dev->a_ref / dev->a_res)) * 1000.0;

      // 6.8 mV/cm
      *distance = volts/6.8;
      return UPM_SUCCESS;
    }

  // UART mode

  char cmd[URM37_MAX_DATA_LEN];
  char resp[URM37_MAX_DATA_LEN];

  // divide degrees by 6 - this is the encoding URM37 uses.
  uint8_t deg = (uint8_t)(degrees / 6);
  if (deg > 46)
    {
      printf("%s: Degrees out of range, must be between 0-270\n",
             __FUNCTION__);
      return UPM_ERROR_OUT_OF_RANGE;
    }

  uint8_t cksum = 0x22 + deg + 0x00;
  cmd[0] = 0x22;
  cmd[1] = deg;
  cmd[2] = 0x00;
  cmd[3] = cksum;

  if (urm37_send_command(dev, cmd, resp) != UPM_SUCCESS)
    {
      printf("%s: urm37_send_command() failed\n", __FUNCTION__);
      return UPM_ERROR_OPERATION_FAILED;
    }

  uint8_t h = (uint8_t) resp[1];
  uint8_t l = (uint8_t) resp[2];

  *distance = (float)((h << 8) | l);

  return UPM_SUCCESS;
}

upm_result_t urm37_get_temperature(urm37_context dev, float* temperature)
{
  if (dev->is_analog_mode)
    {
      printf("%s: Temperature measurement is not available in analog mode\n",
             __FUNCTION__);
      return UPM_ERROR_NOT_SUPPORTED;
    }

  // UART mode
  char cmd[URM37_MAX_DATA_LEN];

  // get temperature sequence
  cmd[0] = 0x11;
  cmd[1] = 0x00;
  cmd[2] = 0x00;
  cmd[3] = 0x11; // cksum

  char resp[URM37_MAX_DATA_LEN];
  if (urm37_send_command(dev, cmd, resp) != UPM_SUCCESS)
    {
      printf("%s: urm37_send_command() failed\n", __FUNCTION__);
      return UPM_ERROR_OPERATION_FAILED;
    }

  uint8_t h = (uint8_t) resp[1];
  uint8_t l = (uint8_t) resp[2];

  *temperature = (float)((h & 0x0f) * 256 + l) / 10.0;

  if (h & 0xf0)
    *temperature *= -1;

  return UPM_SUCCESS;
}

upm_result_t urm37_read_eeprom(urm37_context dev, uint8_t addr, uint8_t* value)
{
  if (dev->is_analog_mode)
    {
      printf("%s: EEPROM is not available in analog mode\n",
             __FUNCTION__);
      return UPM_ERROR_NOT_SUPPORTED;
    }

  if (addr > 0x04)
    {
      printf("Address must be between 0x00-0x04");
      return UPM_ERROR_OUT_OF_RANGE;
    }

  char cmd[URM37_MAX_DATA_LEN];
  uint8_t cksum = 0x33 + addr + 0x00;
  cmd[0] = 0x33;
  cmd[1] = addr;
  cmd[2] = 0x00;
  cmd[3] = cksum;

  char resp[URM37_MAX_DATA_LEN];

  if (urm37_send_command(dev, cmd, resp) != UPM_SUCCESS)
    {
      printf("%s: urm37_send_command() failed\n", __FUNCTION__);
      return UPM_ERROR_OPERATION_FAILED;
    }

  *value = resp[2];
  return UPM_SUCCESS;
}

upm_result_t urm37_write_eeprom(urm37_context dev, uint8_t addr, uint8_t value)
{
  if (dev->is_analog_mode)
    {
      printf("%s: EEPROM is not available in analog mode\n",
             __FUNCTION__);
      return UPM_ERROR_NOT_SUPPORTED;
    }

  if (addr > 0x04)
    {
      printf("Address must be between 0x00-0x04");
      return UPM_ERROR_OUT_OF_RANGE;
    }

  char cmd[URM37_MAX_DATA_LEN];
  uint8_t cksum = 0x44 + addr + value;
  cmd[0] = 0x44;
  cmd[1] = addr;
  cmd[2] = value;
  cmd[3] = cksum;

  char resp[URM37_MAX_DATA_LEN]; // throw away
  if (urm37_send_command(dev, cmd, resp) != UPM_SUCCESS)
    {
      printf("%s: urm37_send_command() failed\n", __FUNCTION__);
      return UPM_ERROR_OPERATION_FAILED;
    }

  return UPM_SUCCESS;
}
