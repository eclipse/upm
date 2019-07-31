/*
 * Authors:
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef UPM_ACCELERATION_H_
#define UPM_ACCELERATION_H_

#ifdef __cplusplus
extern "C" {
#endif

// Acceleration units
typedef enum _upm_acceleration_u {G, METERS_PER_SECOND_SQ, FEET_PER_SECOND_SQ} upm_acceleration_u;

// Acceleration function table
typedef struct _upm_acceleration_ft {
    upm_result_t (*upm_acceleration_set_scale) (void* dev, float* scale);
    upm_result_t (*upm_acceleration_set_offset) (void* dev, float* offset);
    upm_result_t (*upm_acceleration_get_value) (void* dev, float* value, upm_acceleration_u unit);
} upm_acceleration_ft;

#ifdef __cplusplus
}
#endif

#endif /* UPM_ACCELERATION_H_ */
