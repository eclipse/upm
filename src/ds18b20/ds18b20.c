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

#include <assert.h>
#include "ds18b20.h"

// I'd rather use MRAA_UART_OW_ROMCODE_SIZE defined in uart_ow.h, but
// this then can't be used to specify array sizes since it's a static
// const int, rather than a define.  This should be fixed in MRAA (PR
// submitted 9/2016).  Until then, work around it.
#if !defined(MRAA_UART_OW_ROMCODE)
# define ROMCODE_SIZE 8
#else
# define ROMCODE_SIZE MRAA_UART_OW_ROMCODE
#endif

// an internal struct we use to store information on the devices
// found during initialization
struct _ds18b20_info_t {
  uint8_t id[ROMCODE_SIZE];          // 8-byte romcode id
  float temperature;
  DS18B20_RESOLUTIONS_T resolution;
};

// internal utility function forward to read temperature from a single
// device
static float readSingleTemp(const ds18b20_context dev, unsigned int index);

ds18b20_context ds18b20_init(unsigned int uart)
{
  // make sure MRAA is initialized
  int mraa_rv;
  if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
  {
      printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
      return NULL;
  }

  ds18b20_context dev =
    (ds18b20_context)malloc(sizeof(struct _ds18b20_context));

  if (!dev)
    return NULL;

  // zero out context
  memset((void *)dev, 0, sizeof(struct _ds18b20_context));

  dev->ow = NULL;

  if (!(dev->ow = mraa_uart_ow_init(uart)))
    {
      printf("%s: mraa_uart_ow_init() failed.\n", __FUNCTION__);
      ds18b20_close(dev);
      return NULL;
    }

  // iterate through the bus and build up a list of detected DS18B20
  // devices (only)

  mraa_result_t rv;
  if ((rv = mraa_uart_ow_reset(dev->ow)) != MRAA_SUCCESS)
    {
      printf("%s: mraa_uart_ow_reset() failed, no devices detected\n",
             __FUNCTION__);
      ds18b20_close(dev);
      return NULL;
    }

  uint8_t id[ROMCODE_SIZE];

  rv = mraa_uart_ow_rom_search(dev->ow, 1, id);
  if (rv == MRAA_ERROR_UART_OW_NO_DEVICES)
    {
      // shouldn't happen, but....
      printf("%s: mraa_uart_ow_rom_search() failed, no devices detected\n",
             __FUNCTION__);
      ds18b20_close(dev);
      return NULL;
    }

  if (rv == MRAA_ERROR_UART_OW_DATA_ERROR)
    {
      printf("%s: mraa_uart_ow_rom_search() failed, Bus/Data error\n",
             __FUNCTION__);
      ds18b20_close(dev);
      return NULL;
    }

  while (rv == MRAA_SUCCESS)
    {
      // The first byte (id[0]]) is the device type (family) code.  We
      // are only interested in the family code for these devices.

      if ((uint8_t)id[0] == DS18B20_FAMILY_CODE)
        {
          ds18b20_info_t *dsPtr =
            (ds18b20_info_t *)realloc((void *)dev->devices,
                                      sizeof(ds18b20_info_t) *
                                      (dev->numDevices + 1));

          if (!dsPtr)
            {
              printf("%s: realloc(%zu) failed\n",
                     __FUNCTION__,
                     sizeof(ds18b20_info_t) * (dev->numDevices + 1));
              ds18b20_close(dev);
              return NULL;
            }

          dev->devices = dsPtr;
          // copy in the romcode
          memcpy(dev->devices[dev->numDevices].id, id,
                 ROMCODE_SIZE);
          // set defaults for now
          dev->devices[dev->numDevices].temperature = 0.0;
          dev->devices[dev->numDevices].resolution =
            DS18B20_RESOLUTION_12BITS;

          dev->numDevices++;
        }

      // on to the next one
      rv = mraa_uart_ow_rom_search(dev->ow, 0, id);
    }

  if (!dev->numDevices || !dev->devices)
    {
      printf("%s: no DS18B20 devices found on bus\n", __FUNCTION__);
      ds18b20_close(dev);
      return NULL;
    }

  // iterate through the found devices and query their resolutions
  int i;
  for (i=0; i<dev->numDevices; i++)
    {
      // read only the first 5 bytes of the scratchpad
      static const int numScratch = 5;
      uint8_t scratch[numScratch];

      mraa_uart_ow_command(dev->ow, DS18B20_CMD_READ_SCRATCHPAD,
                           dev->devices[i].id);

      int j;
      for (j=0; j<numScratch; j++)
        scratch[j] = (uint8_t)mraa_uart_ow_read_byte(dev->ow);

      // config byte, shift the resolution to bit 0
      scratch[4] >>= _DS18B20_CFG_RESOLUTION_SHIFT;

      switch (scratch[4] & _DS18B20_CFG_RESOLUTION_MASK)
        {
        case 0: dev->devices[i].resolution = DS18B20_RESOLUTION_9BITS; break;
        case 1: dev->devices[i].resolution = DS18B20_RESOLUTION_10BITS; break;
        case 2: dev->devices[i].resolution = DS18B20_RESOLUTION_11BITS; break;
        case 3: dev->devices[i].resolution = DS18B20_RESOLUTION_12BITS; break;
        }

      // reset the bus
      mraa_uart_ow_reset(dev->ow);
    }

  return dev;
}

void ds18b20_close(ds18b20_context dev)
{
  assert(dev != NULL);

  if (dev->devices)
    free(dev->devices);
  if (dev->ow)
    mraa_uart_ow_stop(dev->ow);

  free(dev);
}

void ds18b20_update(const ds18b20_context dev, int index)
{
  assert(dev != NULL);

  if (index >= dev->numDevices)
    {
      printf("%s: device index %d out of range\n", __FUNCTION__, index);
      return;
    }

  // should we update all of them?
  bool doAll = (index < 0) ? true : false;

  if (doAll)
    {
      // if we want to update all of them, we will first send the
      // convert command to all of them, then wait.  This will be
      // faster, timey-wimey wise, then converting, sleeping, and
      // reading each individual sensor.

      int i;
      for (i=0; i<dev->numDevices; i++)
        mraa_uart_ow_command(dev->ow, DS18B20_CMD_CONVERT, dev->devices[i].id);
    }
  else
    mraa_uart_ow_command(dev->ow, DS18B20_CMD_CONVERT, dev->devices[index].id);

  // wait for conversion(s) to finish
  usleep(750000); // 750ms max

  if (doAll)
    {
      int i;
      for (i=0; i<dev->numDevices; i++)
        dev->devices[i].temperature = readSingleTemp(dev, i);
    }
  else
    dev->devices[index].temperature = readSingleTemp(dev, index);
}

// utility function to read temp data from a single sensor
static float readSingleTemp(const ds18b20_context dev, unsigned int index)
{
  assert(dev != NULL);

  if (index >= dev->numDevices)
    {
      printf("%s: device index %d out of range\n", __FUNCTION__, index);
      return 0.0;
    }

  static const int numScratch = 9;
  uint8_t scratch[numScratch];

  // read the 9-byte scratchpad
  mraa_uart_ow_command(dev->ow, DS18B20_CMD_READ_SCRATCHPAD,
                       dev->devices[index].id);
  int i;
  for (i=0; i<numScratch; i++)
    scratch[i] = (uint8_t)mraa_uart_ow_read_byte(dev->ow);

  // validate cksum -- if we get an error, we will warn and simply
  // return the current (previously read) temperature
  uint8_t crc = mraa_uart_ow_crc8(scratch, 8);

  if (crc != scratch[8])
    {
      printf("%s: crc check failed for device %d, returning previously "
             "measured temperature\n", __FUNCTION__, index);
      return dev->devices[index].temperature;
    }

  // check the sign bit(s)
  bool negative = (scratch[1] & 0x80) ? true : false;

  // shift everything into position
  int16_t temp = (scratch[1] << 8) | scratch[0];

  // grab the fractional
  uint8_t frac = temp & 0x0f;

  // depending on the resolution, some frac bits should be ignored, so
  // we mask them off.  For 12bits, all bits are valid so we leve them
  // alone.

  switch (dev->devices[index].resolution)
    {
    case DS18B20_RESOLUTION_9BITS: frac &= 0x08; break;
    case DS18B20_RESOLUTION_10BITS: frac &= 0x0c; break;
    case DS18B20_RESOLUTION_11BITS: frac &= 0x0e; break;
      // use all bits for 12b
    case DS18B20_RESOLUTION_12BITS: break;
    }

  // remove the fractional with extreme prejudice
  temp >>= 4;

  // compensate for sign
  if (negative)
    temp -= 65536; // 2^^16

  // convert
  return ( (float)temp + ((float)frac * 0.0625) );
}

float ds18b20_get_temperature(const ds18b20_context dev, unsigned int index)
{
  assert(dev != NULL);

  if (index >= dev->numDevices)
    {
      printf("%s: device index %d out of range\n", __FUNCTION__, index);
      return 0.0;
    }

  return dev->devices[index].temperature;
}

void ds18b20_set_resolution(const ds18b20_context dev, unsigned int index,
                            DS18B20_RESOLUTIONS_T res)
{
  assert(dev != NULL);

  if (index >= dev->numDevices)
    {
      printf("%s: device index %d out of range\n", __FUNCTION__, index);
      return;
    }

  static const int numScratch = 9;
  uint8_t scratch[numScratch];

  // read the 9-byte scratchpad
  mraa_uart_ow_command(dev->ow, DS18B20_CMD_READ_SCRATCHPAD,
                       dev->devices[index].id);
  int i;
  for (i=0; i<numScratch; i++)
    scratch[i] = (uint8_t)mraa_uart_ow_read_byte(dev->ow);

  // resolution is stored in byte 4
  scratch[4] = ((scratch[4] & ~(_DS18B20_CFG_RESOLUTION_MASK <<
                                _DS18B20_CFG_RESOLUTION_SHIFT))
                | (res << _DS18B20_CFG_RESOLUTION_SHIFT));

  // now, write back, we only write 3 bytes (2-4), no cksum.
  mraa_uart_ow_command(dev->ow, DS18B20_CMD_WRITE_SCRATCHPAD,
                       dev->devices[index].id);
  for (i=0; i<3; i++)
    mraa_uart_ow_write_byte(dev->ow, scratch[i+2]);
}

void ds18b20_copy_scratch_pad(const ds18b20_context dev, unsigned int index)
{
  assert(dev != NULL);

  if (index >= dev->numDevices)
    {
      printf("%s: device index %d out of range\n", __FUNCTION__, index);
      return;
    }

  // issue the command
  mraa_uart_ow_command(dev->ow, DS18B20_CMD_COPY_SCRATCHPAD,
                       dev->devices[index].id);

  sleep(1); // to be safe...
}

void ds18b20_recallEEPROM(const ds18b20_context dev, unsigned int index)
{
  assert(dev != NULL);

  if (index >= dev->numDevices)
    {
      printf("%s: device index %d out of range\n", __FUNCTION__, index);
      return;
    }

  // issue the command
  mraa_uart_ow_command(dev->ow, DS18B20_CMD_RECALL_EEPROM,
                       dev->devices[index].id);

  // issue read timeslots until a '1' is read back, indicating completion
  while (!mraa_uart_ow_bit(dev->ow, 1))
    usleep(100);
}

int ds18b20_devices_found(const ds18b20_context dev)
{
  assert(dev != NULL);

  return dev->numDevices;
}

const uint8_t *ds18b20_get_id(const ds18b20_context dev, unsigned int index)
{
  assert(dev != NULL);

  if (index >= dev->numDevices)
    return NULL;

  static uint8_t id[ROMCODE_SIZE];

  memcpy(id, dev->devices[index].id, ROMCODE_SIZE);
  return id;
}
