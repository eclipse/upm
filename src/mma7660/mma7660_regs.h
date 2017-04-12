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

#define MMA7660_DEFAULT_I2C_BUS 0
#define MMA7660_DEFAULT_I2C_ADDR 0x4c

#ifdef __cplusplus
extern "C" {
#endif

    // MMA7660 registers
    typedef enum {
        MMA7660_REG_XOUT       = 0x00,
        MMA7660_REG_YOUT       = 0x01,
        MMA7660_REG_ZOUT       = 0x02,
        MMA7660_REG_TILT       = 0x03,
        MMA7660_REG_SRST       = 0x04, // Sampling Rate Status
        MMA7660_REG_SPCNT      = 0x05, // sleep count
        MMA7660_REG_INTSU      = 0x06, // Interrupt setup
        MMA7660_REG_MODE       = 0x07, // operating mode
        MMA7660_REG_SR         = 0x08, // auto-wake/sleep, SPS, and debounce
        MMA7660_REG_PDET       = 0x09, // tap detection
        MMA7660_REG_PD         = 0x0a  // tap debounce count
        // 0x0b-0x1f reserved
    } MMA7660_REG_T;

    // interrupt enable register bits
    typedef enum {
        MMA7660_INTR_NONE          = 0x00, // disabled
        MMA7660_INTR_FBINT         = 0x01, // front/back
        MMA7660_INTR_PLINT         = 0x02, // up/down/right/left
        MMA7660_INTR_PDINT         = 0x04, // tap detection
        MMA7660_INTR_ASINT         = 0x08, // exit auto-sleep
        MMA7660_INTR_GINT          = 0x10, // measurement intr
        MMA7660_INTR_SHINTZ        = 0x20, // shake on Z
        MMA7660_INTR_SHINTY        = 0x40, // shake on Y
        MMA7660_INTR_SHINTX        = 0x80  // shake on X
    } MMA7660_INTR_T;

    // operating mode register bits
    typedef enum {
        MMA7660_MODE_MODE          = 0x01, // determines mode with MODE_TON
        // 0x02 reserved
        MMA7660_MODE_TON           = 0x04, // determines mode with MODE_MODE
        MMA7660_MODE_AWE           = 0x08, // auto-wake
        MMA7660_MODE_ASE           = 0x10, // auto-sleep
        MMA7660_MODE_SCPS          = 0x20, // sleep count prescale
        MMA7660_MODE_IPP           = 0x40, // intr out push-pull/open drain
        MMA7660_MODE_IAH           = 0x80  // intr active low/high
    } MMA7660_MODE_T;

    // tilt BackFront (BF) bits
    typedef enum {
        MMA7660_BF_UNKNOWN          = 0x00,
        MMA7660_BF_LYING_FRONT      = 0x01,
        MMA7660_BF_LYING_BACK       = 0x02
    } MMA7660_TILT_BF_T;

    // tilt LandscapePortrait (LP) bits
    typedef enum {
        MMA7660_LP_UNKNOWN          = 0x00,
        MMA7660_LP_LANDSCAPE_LEFT   = 0x01,
        MMA7660_LP_LANDSCAPE_RIGHT  = 0x02,
        MMA7660_LP_VERT_DOWN        = 0x05,
        MMA7660_LP_VERT_UP          = 0x06
    } MMA7660_TILT_LP_T;

    // sample rate (auto-sleep) values
    typedef enum {
        MMA7660_AUTOSLEEP_120   = 0x00,
        MMA7660_AUTOSLEEP_64    = 0x01,
        MMA7660_AUTOSLEEP_32    = 0x02,
        MMA7660_AUTOSLEEP_16    = 0x03,
        MMA7660_AUTOSLEEP_8     = 0x04,
        MMA7660_AUTOSLEEP_4     = 0x05,
        MMA7660_AUTOSLEEP_2     = 0x06,
        MMA7660_AUTOSLEEP_1     = 0x07
    } MMA7660_AUTOSLEEP_T;

#ifdef __cplusplus
}
#endif
