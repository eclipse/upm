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

#ifndef UPM_ANGLE_H_
#define UPM_ANGLE_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum _upm_angle_u {DEGREES, RADIANS} upm_angle_u;

typedef struct _upm_angle_ft {
    upm_result_t (*upm_angle_get_value) (void* dev, float* value, upm_angle_u unit);
} upm_angle_ft;

#ifdef __cplusplus
}
#endif

#endif /* UPM_ANGLE_H_ */ 
