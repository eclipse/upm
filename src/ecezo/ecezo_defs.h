/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
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

// our maximum buffer size
#define ECEZO_MAX_BUFFER_LEN (64)

    // calibration commands
    typedef enum {
        ECEZO_CALIBRATE_CLEAR            = 0, // clear calibration
        ECEZO_CALIBRATE_DRY,                  // cal dry point
        ECEZO_CALIBRATE_ONE,                  // single point cal EC value
        ECEZO_CALIBRATE_LOW,                  // 2-point cal, LOW EC value
        ECEZO_CALIBRATE_HIGH                  // 2-point cal, HIGH EC value
    } ECEZO_CALIBRATION_T;

#ifdef __cplusplus
}
#endif
