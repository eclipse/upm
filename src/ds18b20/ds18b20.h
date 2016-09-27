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
#pragma once

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <mraa/uart_ow.h>

#include "upm.h"

#ifdef __cplusplus
extern "C" {
#endif

// The family code for these devices.  We handle all of them that
// are found on the bus.
#define DS18B20_FAMILY_CODE 0x28

  /**
   * @file ds18b20.h
   * @library ds18b20
   * @brief C API for the DS18B20 1-Wire Temperature Sensor
   *
   * @include ds18b20.c
   */

  // forward declaration
  typedef struct _ds18b20_info_t ds18b20_info_t;

  /**
   * Device context
   */
  typedef struct _ds18b20_context {
    mraa_uart_ow_context ow;

    // number of devices found
    int numDevices;

    // list of allocated ds18b20_info_t instances
    ds18b20_info_t *devices;
  } *ds18b20_context;

  // commands
  typedef enum {
    DS18B20_CMD_CONVERT                 = 0x44, // start a temp conversion
    DS18B20_CMD_WRITE_SCRATCHPAD        = 0x4e,
    DS18B20_CMD_READ_SCRATCHPAD         = 0xbe,
    DS18B20_CMD_COPY_SCRATCHPAD         = 0x48, // copy scratchpad to EEPROM
    DS18B20_CMD_RECALL_EEPROM           = 0xb8, // copy EEPROM to scratchpad
    DS18B20_CMD_READ_POWER_SUPPLY       = 0xb4  // parasitically powered?
  } DS18B20_CMD_T;

  // config register (scratchpad[4])
  typedef enum {
    DS18B20_CFG_RESOLUTION_R0                   = 0x20,
    DS18B20_CFG_RESOLUTION_R1                   = 0x40,
    _DS18B20_CFG_RESOLUTION_MASK                = 3,
    _DS18B20_CFG_RESOLUTION_SHIFT               = 5

    // all other bits reserved and non-writable
  } DS18B20_CFG_BITS_T;

  typedef enum {
    DS18B20_RESOLUTION_9BITS                    = 0, // 93.75ms (tconv/8)
    DS18B20_RESOLUTION_10BITS                   = 1, // 187.5 (tconv/4)
    DS18B20_RESOLUTION_11BITS                   = 2, // 375ms (tconv/2)
    DS18B20_RESOLUTION_12BITS                   = 3  // 750ms (tconv)
  } DS18B20_RESOLUTIONS_T;

  /**
   * This function will initilaize and search the 1-wire bus and store
   * information on each DS18B20 device detected on the bus.  If no
   * devices are found, NULL is returned.  Once this function
   * completes successfully, you can use ds18b20_devices_found() to
   * determine how many devices were detected.
   *
   * @return device context, or NULL on error.
   */
  ds18b20_context ds18b20_init(unsigned int uart);

  /**
   * Close the device and deallocate all resources.
   */
  void ds18b20_close(ds18b20_context dev);

  /**
   * Update our stored temperature for a device.  This method must
   * be called prior to ds18b20_get_temperature().
   *
   * @param index The device index to access (starts at 0).  Specify
   * -1 to query all detected devices.  Default: -1
   */
  void ds18b20_update(const ds18b20_context dev, int index);

  /**
   * Get the current temperature.  ds18b20_update() must have been
   * called prior to calling this method.
   *
   * @param index The device index to access (starts at 0).
   * @return The last temperature reading in Celsius.
   */
  float ds18b20_get_temperature(const ds18b20_context dev,
                                unsigned int index);

  /**
   * Set the device resolution for a device.  These devices support
   * 9, 10, 11, and 12 bits of resolution, with the default from the
   * factory at 12 bits.
   *
   * @param index The device index to access (starts at 0).
   * @param res One of the DS18B20_RESOLUTIONS_T values
   */
  void ds18b20_set_resolution(const ds18b20_context dev, unsigned int index,
                              DS18B20_RESOLUTIONS_T res);

  /**
   * Copy the device's scratchpad memory to the EEPROM.  This
   * includes the configuration byte (resolution).
   *
   * @param index The device index to access (starts at 0).
   */
  void ds18b20_copy_scratchpad(const ds18b20_context dev, unsigned int index);

  /**
   * Copy the device's EEPROM memory to the scratchpad.  This method
   * will return when the copy completes.  This operation is
   * performed by the device automatically on power up, so it is
   * rarely needed.
   *
   * @param index The device index to access (starts at 0).
   */
  void ds18b20_recall_eeprom(const ds18b20_context dev, unsigned int index);

  /**
   * This method will return the number of DS18B20 devices that were
   * found on the bus by ds18b20_init().
   *
   * @return number of DS18B20's that were found on the bus
   */
  int ds18b20_devices_found(const ds18b20_context dev);

  /**
   * Return an 8 byte string representing the unique device ID (1-wire
   * romcode) for a given device index.  The pointer returned is
   * statically allocated and will be overwritten on each call.
   *
   * @param index The device index to access (starts at 0).
   * @return pointer to 8 byte DS18B20_ROMCODE_T representing the 1-wire
   * device's unique romcode, or NULL on error.
   */
  const uint8_t *get_id(const ds18b20_context dev, unsigned int index);

#ifdef __cplusplus
}
#endif
