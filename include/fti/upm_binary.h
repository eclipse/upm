/*
 * Authors: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef UPM_BINARY_H_
#define UPM_BINARY_H_

#ifdef __cplusplus
extern "C" {
#endif

// Binary function table.  This table is for simple sensors that just
// return a single TRUE/FALSE, ON/OFF type values.  It can also be
// used to set a simple binary value.
typedef struct _upm_binary_ft {
    upm_result_t (*upm_binary_get_value) (void* dev, bool* value);
    upm_result_t (*upm_binary_set_value) (void* dev, bool value);
} upm_binary_ft;

#ifdef __cplusplus
}
#endif

#endif /* UPM_BINARY_H_ */
