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
#ifndef UPM_PH_H_
#define UPM_PH_H_

#include "upm_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* pH function table */
typedef struct _upm_ph_ft
{
    /* Set sensor offset in raw counts */
    upm_result_t (*upm_ph_set_offset) (const void* dev, float offset);
    /* Set sensor scale in raw counts */
    upm_result_t (*upm_ph_set_scale) (const void* dev, float scale);
    /* Read sensor value, return pH */
    upm_result_t (*upm_ph_get_value) (const void* dev, float* value);
} upm_ph_ft;

#ifdef __cplusplus
}
#endif

#endif /* UPM_PH_H_ */
