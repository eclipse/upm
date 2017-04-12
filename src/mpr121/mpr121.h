/*
 * Author: Jon Trulson <jtrulson@ics.com>
 *         Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2014 Intel Corporation.
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

#ifndef MPR121_H_
#define MPR121_H_

#include <stdint.h>
#include "upm.h"
#include "mraa/i2c.h"

#define MPR121_I2C_BUS     0
#define MPR121_DEFAULT_I2C_ADDR    0x5a

/**
 * @file mpr121.h
 * @library mpr121
 * @brief C API for the MPR121 Touch Sensor
 *
 * @include mpr121.c
 */

/**
 * device context
 */
typedef struct _mpr121_context {
    mraa_i2c_context    i2c;
    int                 bus;
    uint8_t             address;
    uint8_t             over_current_fault;
} *mpr121_context;

/**
 * MPR121 Init function
 *
 * @param bus i2c bus
 * @param address i2c address of the sensor
 * @return void* pointer to the sensor struct
 */
mpr121_context mpr121_init(int bus, uint8_t address);

/**
 * MPR121 Close function
 *
 * @param dev pointer to sensor struct
 */
void mpr121_close(mpr121_context dev);

/**
 * Sets up a default configuration, based on Application Note 3944
 * (AN3944):
 * http://cache.freescale.com/files/sensors/doc/app_note/AN3944.pdf
 *
 * After configuration, the sensor is left in the run state.
 *
 * @param dev pointer to the sensor struct
 * @return upm_result_t UPM success/error code
 */
upm_result_t mpr121_config_an3944(mpr121_context dev);

/**
 * Function to read the individual button values
 *
 * @param dev pointer to the sensor struct
 * @param states Pointer to store the states of the individual buttons
 * @param num used to retrieve the state of a particular button
 * @return upm_result_t UPM success/error code
 */
upm_result_t mpr121_read_buttons(mpr121_context dev, uint32_t* states, int num);

/**
 * Function used to write multiple bytes to the i2c bus
 *
 * @param dev sensor struct
 * @param reg i2c register to read from
 * @param *buffer pointer that contains the values that have to be
 * written to the registers
 * @param len length of the values that have to be written to the regs
 * @return upm_result_t UPM success/error code
 */
upm_result_t mpr121_write_bytes(mpr121_context dev, uint8_t reg,
                                uint8_t *buffer, int len);

/**
 * Function used to read multiple bytes from the i2c bus
 *
 * @param dev sensor struct
 * @param reg i2c register to read from
 * @param *buffer pointer that contains the values that have been
 * read from the registers
 * @param len length of the values that have to be read
 * @return upm_result_t UPM success/error code
 */

upm_result_t mpr121_read_bytes(mpr121_context dev, uint8_t reg,
                               uint8_t *buffer, int len);

#endif /* MPR121_H_ */
