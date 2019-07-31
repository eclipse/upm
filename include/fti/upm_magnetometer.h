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
#ifndef UPM_MAGNETOMETER_H_
#define UPM_MAGNETOMETER_H_

#ifdef __cplusplus
extern "C" {
#endif

// Magnetometer function table
typedef struct _upm_magnetometer_ft {
    upm_result_t (*upm_magnetometer_set_scale) (void* dev, float* scale);
    upm_result_t (*upm_magnetometer_set_offset) (void* dev, float* offset);
    upm_result_t (*upm_magnetometer_get_value) (void* dev, float* value);
} upm_magnetometer_ft;

#ifdef __cplusplus
}
#endif

#endif /* UPM_MAGNETOMETER_H_ */
