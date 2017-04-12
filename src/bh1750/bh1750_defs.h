/*
 * Authors: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * The MIT License
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

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

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

#ifdef __cplusplus
}
#endif // __cplusplus
