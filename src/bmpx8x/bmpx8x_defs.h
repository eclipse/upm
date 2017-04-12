/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * This driver was rewritten based on the original driver written by:
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
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

#define BMPX8X_DEFAULT_I2C_BUS   0
#define BMPX8X_DEFAULT_I2C_ADDR  0x77
#define BMPX8X_DEFAULT_CHIPID    0x55
// special reset byte
#define BMPX8X_RESET_BYTE        0xb6

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * BMPX8X registers.
     */
    typedef enum {
        BMPX8X_CAL_AC1           = 0xaa,  // Calibration data (16
                                          // bits, msb/lsb)
        BMPX8X_CAL_AC2           = 0xac,
        BMPX8X_CAL_AC3           = 0xae,
        BMPX8X_CAL_AC4           = 0xb0,
        BMPX8X_CAL_AC5           = 0xb2,
        BMPX8X_CAL_AC6           = 0xb4,
        BMPX8X_CAL_B1            = 0xb6,
        BMPX8X_CAL_B2            = 0xb8,
        BMPX8X_CAL_MB            = 0xba,
        BMPX8X_CAL_MC            = 0xbc,
        BMPX8X_CAL_MD            = 0xbe,

        BMPX8X_CTRL_MEAS         = 0xf4, // command reg

        BMPX8X_OUTDATA_MSB       = 0xf6,
        BMPX8X_OUTDATA_LSB       = 0xf7,
        BMPX8X_OUTDATA_XLSB      = 0xf8,

        BMPX8X_RESET             = 0xe0,

        BMPX8X_CHIP_ID           = 0xd0
    } BMPX8X_REGS_T;

    /**
     * BMPX8X_CTRL_MEAS register bits
     */
    typedef enum {
        BMPX8X_CTRL_MEAS_CMD0    = 0x01, // measurement command
        BMPX8X_CTRL_MEAS_CMD1    = 0x02,
        BMPX8X_CTRL_MEAS_CMD2    = 0x04,
        BMPX8X_CTRL_MEAS_CMD3    = 0x08,
        BMPX8X_CTRL_MEAS_CMD4    = 0x10,
        _BMPX8X_CTRL_MEAS_CMD_MASK  = 31,
        _BMPX8X_CTRL_MEAS_CMD_SHIFT = 0,

        BMPX8X_CTRL_MEAS_SCO     = 0x20, // start conversion status

        BMPX8X_CTRL_MEAS_OSS0    = 0x40, // oversampling (precision)
        BMPX8X_CTRL_MEAS_OSS1    = 0x80,
        _BMPX8X_CTRL_MEAS_OSS_MASK  = 3,
        _BMPX8X_CTRL_MEAS_OSS_SHIFT = 6,
    } BMPX8X_CTRL_MEAS_BITS_T;

    /**
     * BMPX8X_CTRL_MEAS_CMD commands.
     */
    typedef enum {
        BMPX8X_CMD_READ_TEMP     = 0x2e,
        BMPX8X_CMD_READ_PRESSURE = 0x34
    } BMPX8X_CMD_T;

    /**
     * BMPX8X_CTRL_MEAS_OSS, Oversampling ratio values.
     */
    typedef enum {
        BMPX8X_OSS_ULTRALOWPOWER = 0,
        BMPX8X_OSS_STANDARD      = 1,
        BMPX8X_OSS_HIGHRES       = 2,
        BMPX8X_OSS_ULTRAHIGHRES  = 3
    } BMPX8X_OSS_T;


#ifdef __cplusplus
}
#endif
