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

#ifndef UPM_POTENTIOMETER_H_
#define UPM_POTENTIOMETER_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum _upm_potentiometer_u {VOLTAGE} upm_potentiometer_u;

typedef struct _upm_potentiometer_ft {
    upm_result_t (*upm_potentiometer_get_value) (void* dev, float* value, upm_potentiometer_u unit);
} upm_potentiometer_ft;

#ifdef __cplusplus
}
#endif

#endif /* UPM_POTENTIOMETER_H_ */ 
