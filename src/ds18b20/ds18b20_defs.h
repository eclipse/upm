/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
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

// The family code for these devices.  We handle all of them that
// are found on the bus.
#define DS18B20_FAMILY_CODE 0x28

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

#ifdef __cplusplus
}
#endif
