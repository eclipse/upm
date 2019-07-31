/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
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
