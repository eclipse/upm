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

// default I2C address
#define MS5803_DEFAULT_I2C_ADDR 0x76

#define MS5803_MAX_COEFFICIENTS (8)

    // valid commands
    typedef enum {
        MS5803_CMD_RESET                = 0x1e,

        // D1 = pressure
        MS5803_CMD_CONVERT_D1_OSR_256   = 0x40,
        MS5803_CMD_CONVERT_D1_OSR_512   = 0x42,
        MS5803_CMD_CONVERT_D1_OSR_1024  = 0x44,
        MS5803_CMD_CONVERT_D1_OSR_2048  = 0x46,
        MS5803_CMD_CONVERT_D1_OSR_4096  = 0x48,

        // D2 = temperature
        MS5803_CMD_CONVERT_D2_OSR_256   = 0x50,
        MS5803_CMD_CONVERT_D2_OSR_512   = 0x52,
        MS5803_CMD_CONVERT_D2_OSR_1024  = 0x54,
        MS5803_CMD_CONVERT_D2_OSR_2048  = 0x56,
        MS5803_CMD_CONVERT_D2_OSR_4096  = 0x58,

        // ADC read
        MS5803_CMD_ADC_READ             = 0x00,

        // PROM read.  Bits 1, 2, and 3 indicate the address. Bit 0 is
        // always 0 (in all commands).  There are 7 PROM locations,
        // each 2 bytes in length.  These locations store factory
        // loaded compensation coefficients.
        MS5803_CMD_PROM_READ            = 0xa0
    } MS5803_CMD_T;

    // output sampling resolution for temperature and pressure.  We
    // set the numeric values here to indicate the required wait time
    // for each in milliseconds (rounded up from the datasheet
    // maximums), so do not change these numbers.
    typedef enum {
        MS5803_OSR_256                  = 1, // 1ms
        MS5803_OSR_512                  = 2,
        MS5803_OSR_1024                 = 3,
        MS5803_OSR_2048                 = 5,
        MS5803_OSR_4096                 = 10
    } MS5803_OSR_T;

#ifdef __cplusplus
}
#endif
