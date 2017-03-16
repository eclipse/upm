/*
 * Author: Noel Eck <noel.eck@intel.com>
 * Copyright (c) 2015 Intel Corporation.
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
