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

    /**
     * Enum to specify the direction of a motor
     */
    typedef enum {
      ULN200XA_DIR_CW   = 0x01,
      ULN200XA_DIR_CCW  = 0x02
    } ULN200XA_DIRECTION_T;

#ifdef __cplusplus
}
#endif
