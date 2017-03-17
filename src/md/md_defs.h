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

#ifdef __cplusplus
extern "C" {
#endif

#define MD_I2C_BUS 0
#define MD_DEFAULT_I2C_ADDR 0x0f

// This is a NOOP value used to pad packets
#define MD_NOOP 0x01


    // MD registers
    typedef enum {
        MD_REG_SET_SPEED           = 0x82,
        MD_REG_SET_PWM_FREQ        = 0x84,
        MD_REG_SET_DIRECTION       = 0xaa,
        MD_REG_SET_MOTOR_A         = 0xa1, // not documented
        MD_REG_SET_MOTOR_B         = 0xa5, // not documented
        MD_REG_STEPPER_ENABLE      = 0x1a,
        MD_REG_STEPPER_DISABLE     = 0x1b,
        MD_REG_STEPPER_NUM_STEPS   = 0x1c
    } MD_REG_T;

    // legal directions for the stepper
    typedef enum {
        MD_STEP_DIR_CCW    = 0x01,
        MD_STEP_DIR_CW     = 0x00
    } MD_STEP_DIRECTION_T;

    // legal directions for individual DC motors
    typedef enum {
        MD_DIR_CCW    = 0x02,
        MD_DIR_CW     = 0x01
    } MD_DC_DIRECTION_T;

    // stepper modes
    typedef enum {
        MD_STEP_MODE1 = 0x00,
        MD_STEP_MODE2 = 0x01
    } MD_STEP_MODE_T;

#ifdef __cplusplus
}
#endif
