/*
 * Author: Noel Eck <noel.eck@intel.com>
 * Copyright (c) 2015 Intel Corporation.
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

#include <errno.h>
#include <stdint.h>

#include "mraa/i2c.h"
#include "upm.h"

#include "ims_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file ims.h
 * @library ims
 * @brief C API for the Catnip Electronics I2C moisture sensor.  This sensor
 * must run at 100 kHz.
 *
 * @include ims.c
 */

/**
 * device context
 */
typedef struct {
    /* mraa i2c context */
    mraa_i2c_context _i2c_context;
} ims_context;

/**
 * Initialize sensor
 * @param i2c_bus Target I2C bus
 * @param i2c_address Target I2C address (default is 0x20)
 * @return sensor context pointer
 */
ims_context* ims_init(int16_t i2c_bus, int16_t i2c_address);

/**
 * Sensor close method.
 * Cleans up any memory held by this device
 * @param sensor context pointer
 */
void ims_close(ims_context* dev);

/**
 * Read I2C Moisture Sensor registers
 * @param dev Sensor context pointer
 * @param cmd Read command
 * @param rd_data Data returned from sensor (1 or 2 bytes depending on cmd)
 * @return Function result code
 */
upm_result_t ims_read(const ims_context* dev, IMS_RD_COMMAND cmd, uint16_t* rd_data);

/**
 * Write I2C Moisture Sensor registers
 * @param dev Sensor context pointer
 * @param cmd Write command
 * @param wr_data Target data to write (only used for IMS_SET_ADDRESS)
 * @return Function result code
 */
upm_result_t ims_write(const ims_context* dev, IMS_WR_COMMAND cmd, uint8_t wr_data);

/**
 * Get sensor version
 * @param dev Sensor context pointer
 * @param rd_data Sensor version
 * @return Function result code
 */
upm_result_t ims_get_version(const ims_context* dev, uint16_t* rd_data);

/**
 * Get moisture reading from sensor
 * @param dev Sensor context pointer
 * @param rd_data Unitless, relative capacitance value (used to determine moisture)
 * @return Function result code
 */
upm_result_t ims_get_moisture(const ims_context* dev, uint16_t* rd_data);

/**
 * Get light reading from LED on device.  The technical data for the I2C
 * moisture sensor specifies a 3 second wait.  Loop for 3 seconds checking
 * the GET_BUSY register.  IF the sensor is NOT ready after 3 seconds,
 * assume there is NO light and return a max uint16_t (dark) value.
 * @param dev Sensor context pointer
 * @param rd_data Unitless, relative value for brightness
 *        dark (0xFFFF) ---> light (0x0000)
 * @return Function result code
 */
upm_result_t ims_get_light(const ims_context* dev, uint16_t* rd_data);

/**
 * Get temperature reading from device
 * @param dev Sensor context pointer
 * @param rd_data Temperature in degrees Celsius * 10
 *        ie, 256 = 25.6 C
 * @return Function result code
 */
upm_result_t ims_get_temperature(const ims_context* dev, uint16_t* rd_data);

/**
 * Reset sensor
 * @param dev Sensor context pointer
 * @return Function result code
 */
upm_result_t ims_reset(const ims_context* dev);

/**
 * Set I2C address AND reset sensor
 * @param dev Sensor context pointer
 * @param address_new New I2C for device
 * @return Function result code
 */
upm_result_t ims_reset_i2c_address(const ims_context* dev, uint8_t address_new);

/**
 * Put device into low-power mode.  Device wakes on any I2C command.
 * @param dev Sensor context pointer
 * @return Function result code
 */
upm_result_t ims_sleep(const ims_context* dev);

#ifdef __cplusplus
}
#endif
