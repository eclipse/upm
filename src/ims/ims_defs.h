/*
 * Author: Noel Eck <noel.eck@intel.com>
 * Copyright (c) 2015 Intel Corporation.
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

#define IMS_ADDRESS_DEFAULT 0x20

 /* @brief Moisture sensor I2C READ commands */
typedef enum _IMS_RD_COMMAND {
        /* Read capacitance (moisture) register */
        IMS_GET_CAPACITANCE = 0x00, /*    (r) 2 */
        /* Read I2C address register */
        IMS_GET_ADDRESS     = 0x02, /*    (r) 1 */
        /* Read light register (requires write to IMS_MEASURE_LIGHT) */
        IMS_GET_LIGHT       = 0x04, /*    (r) 2 */
        /* Read temperature register */
        IMS_GET_TEMPERATURE = 0x05, /*    (r) 2 */
        /* Read version register */
        IMS_GET_VERSION     = 0x07, /*    (r) 1 */
        /* Read busy register (0 = ready, 1 = sampling) */
        IMS_GET_BUSY        = 0x09, /*    (r) 1 */
} IMS_RD_COMMAND;

 /* @brief Moisture sensor I2C WRITE commands */
typedef enum {
        /* Write I2C address register (latched w/IMS_RESET) */
        IMS_SET_ADDRESS     = 0x01, /*    (w) 1 */
        /* Initiate light measurement */
        IMS_MEASURE_LIGHT   = 0x03, /*    (w) 0 */
        /* Reset device */
        IMS_RESET           = 0x06, /*    (w) 0 */
        /* Sleep microcontroller, wake on any I2C request */
        IMS_SLEEP           = 0x08, /*    (w) 0 */
} IMS_WR_COMMAND;


#ifdef __cplusplus
}
#endif
