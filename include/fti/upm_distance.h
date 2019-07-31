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
#ifndef UPM_DISTANCE_H_
#define UPM_DISTANCE_H_

#ifdef __cplusplus
extern "C" {
#endif

// Distance units
typedef enum _upm_distance_u {CENTIMETER, INCH} upm_distance_u;

// Distance function table
typedef struct _upm_dist_ft {
    upm_result_t (*upm_distance_get_value) (void* dev, float* value, upm_distance_u unit);
} upm_distance_ft;

#ifdef __cplusplus
}
#endif

#endif /* UPM_SENSOR_DISTANCE_H_ */
