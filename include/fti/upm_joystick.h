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
#ifndef UPM_JOYSTICK_H_
#define UPM_JOYSTICK_H_

#include "upm_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Joytick function table */
typedef struct _upm_joystick_ft
{
    /* Set sensor offset in raw counts */
    upm_result_t (*upm_joystick_set_offset_x) (const void* dev, float offset);
    /* Set sensor offset in raw counts */
    upm_result_t (*upm_joystick_set_offset_y) (const void* dev, float offset);
    /* Set sensor scale in raw counts */
    upm_result_t (*upm_joystick_set_scale_x) (const void* dev, float scale);
    /* Set sensor scale in raw counts */
    upm_result_t (*upm_joystick_set_scale_y) (const void* dev, float scale);
    /* Read sensor value, return normalized value */
    upm_result_t (*upm_joystick_get_value_x) (const void* dev, float* value);
    /* Read sensor value, return normalized value */
    upm_result_t (*upm_joystick_get_value_y) (const void* dev, float* value);
} upm_joystick_ft;

#ifdef __cplusplus
}
#endif

#endif /* UPM_JOYSTICK_H_ */
