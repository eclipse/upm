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
#ifndef UPM_COMPASS_H_
#define UPM_COMPASS_H_

#ifdef __cplusplus
extern "C" {
#endif

// Compass function table
typedef struct _upm_compass_ft {
    upm_result_t (*upm_compass_set_scale) (void* dev, float scale);
    upm_result_t (*upm_compass_set_offset) (void* dev, float offset);
    // returns a heading in degrees
    upm_result_t (*upm_compass_get_value) (void* dev, float *value);
} upm_compass_ft;

#ifdef __cplusplus
}
#endif

#endif /* UPM_COMPASS_H_ */
