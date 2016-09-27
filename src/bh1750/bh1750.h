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

#pragma once

#include <stdint.h>

#include "mraa/i2c.h"

#include "upm.h"
#include "upm_types.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @file bh1750.h
 * @library bh1750
 * @brief C API for the BH1750 Light Sensor
 *
 * @include bh1750.c
 */

#define BH1750_DEFAULT_I2C_BUS              0
#define BH1750_DEFAULT_I2C_ADDR             0x23

// BH1750 commands

#define BH1750_CMD_POWER_DOWN               0x00
#define BH1750_CMD_POWER_UP                 0x01

#define BH1750_CMD_RESET                    0x07

// continuous modes
#define BH1750_CMD_CONT_H_RES_MODE1         0x10 // 1 lx resolution
#define BH1750_CMD_CONT_H_RES_MODE2         0x11 // .5 lx resolution
#define BH1750_CMD_CONT_L_RES_MODE          0x13 // 4 lx resolution

// one-time modes
#define BH1750_CMD_ONETIME_H_RES_MODE1      0x20
#define BH1750_CMD_ONETIME_H_RES_MODE2      0x21
#define BH1750_CMD_ONETIME_L_RES_MODE       0x23

// max measurement time in ms (for H modes)
#define BH1750_MAX_MEAS_TIME_H              180

// max measurement time in ms (for L modes)
#define BH1750_MAX_MEAS_TIME_L              30

// an enum for the operating mode to pass to init
typedef enum {
  BH1750_OPMODE_H1_CONT, // continuous 1 lx high resolution
  BH1750_OPMODE_H2_CONT, // continuous .5 lx high resolution
  BH1750_OPMODE_L_CONT,  // continuous 4 lx low resolution
  BH1750_OPMODE_H1_ONCE, // onetime 1 lx high resolution
  BH1750_OPMODE_H2_ONCE, // onetime .5 lx high resolution
  BH1750_OPMODE_L_ONCE,  // onetime 4 lx low resolution
} BH1750_OPMODES_T;
    
/**
 * device context
 */
typedef struct _bh1750_context
{
  int                 bus;
  mraa_i2c_context    i2c;
  
  // these are set by bh1750_set_opmode()
  uint8_t             opmode;
  bool                is_continuous;
  int                 delayms;
} *bh1750_context;

/**
 * Initialize the BH1750
 *
 * @param bus I2C bus
 * @param addr I2C address
 * @param mode operating mode, one of the BH1750_OPMODES_T values
 * @return bh1750_context for the new device context
 */
bh1750_context bh1750_init(int bus, uint8_t addr, BH1750_OPMODES_T mode);

/**
 * Sensor destructor function.  Frees any allocated resources.
 *
 * @param dev The device context
 */
void bh1750_close(const bh1750_context dev);

/**
 * Gets the Lux value.
 *
 * @param dev The device context
 * @param lux float pointer in which to store the lux value
 * @return upm_result_t UPM success/error code
 */
upm_result_t bh1750_get_lux(const bh1750_context dev, float* lux);

/**
 * Power up the device.
 *
 * @param dev The device context
 * @return true if the command was successful, false otherwise
 */
bool bh1750_power_up(const bh1750_context dev);

/**
 * Power down the device.
 *
 * @param dev The device context
 * @return true if the command was successful, false otherwise
 */
bool bh1750_power_down(const bh1750_context dev);

/**
 * Reset the device.  This doesn't really have much purpose.  The
 * device must be powered up for this command to work.  In addition,
 * this command will simply clear the measurement register to 0.
 *
 * @param dev The device context
 * @return true if the command was successful, false otherwise
 */
bool bh1750_reset(const bh1750_context dev);

/**
 * Write a command to the device via I2C.
 *
 * @param dev The device context
 * @param cmd The command to write, one of the BH1750_CMD* values
 * @return upm_result_t UPM success/error code
 */
upm_result_t bh1750_send_command(const bh1750_context dev, uint8_t cmd);

/**
 * Read the 2 result bytes from the device via I2C.
 *
 * @param dev The device context
 * @param data Data read in from the device as a uint16_t
 * @return upm_result_t UPM success/error code
 */
upm_result_t bh1750_read_data(const bh1750_context dev, uint16_t* data);

/**
 * Setup the device context parameters and the device to match the
 * selected operating mode.
 *
 * @param dev The device context
 * @param mode operating mode, one of the BH1750_OPMODES_T values
 * @return upm_result_t UPM success/error code
 */
upm_result_t bh1750_set_opmode(const bh1750_context dev,
                               BH1750_OPMODES_T mode);

#ifdef __cplusplus
}
#endif // __cplusplus
