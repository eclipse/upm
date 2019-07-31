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
#ifndef UPM_HUMIDITY_H_
#define UPM_HUMIDITY_H_

#ifdef __cplusplus
extern "C" {
#endif

// Humidity function table
typedef struct _upm_humidity_ft {
    upm_result_t (*upm_humidity_set_scale) (void* dev, float scale);
    upm_result_t (*upm_humidity_set_offset) (void* dev, float offset);
    upm_result_t (*upm_humidity_get_value) (void* dev, float* value);
} upm_humidity_ft;

#ifdef __cplusplus
}
#endif

#endif /* UPM_HUMIDITY_H_ */
