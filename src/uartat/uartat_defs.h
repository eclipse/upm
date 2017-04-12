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
