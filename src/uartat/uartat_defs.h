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

    // possible flow control methods
    typedef enum {
        UARTAT_FLOW_CONTROL_NONE           = 0,
        UARTAT_FLOW_CONTROL_HARD,          // hardware flow control
        UARTAT_FLOW_CONTROL_SOFT           // software flow control
    } UARTAT_FLOW_CONTROL_T;


    // Numeric response codes via ITU-T V25Ter recommendations
    typedef enum {
        UARTAT_RESPONSE_CODE_OK            = 0,
        UARTAT_RESPONSE_CODE_CONNECT       = 1,
        UARTAT_RESPONSE_CODE_RING          = 2,
        UARTAT_RESPONSE_CODE_NO_CARRIER    = 3,
        UARTAT_RESPONSE_CODE_ERROR         = 4,
        // 5 unassigned
        UARTAT_RESPONSE_CODE_NO_DIALTONE   = 6,
        UARTAT_RESPONSE_CODE_BUSY          = 7,
        UARTAT_RESPONSE_CODE_NO_ANSWER     = 8
    } UARTAT_RESPONSE_CODE_T;

#ifdef __cplusplus
}
#endif
