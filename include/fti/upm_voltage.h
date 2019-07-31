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
#ifndef UPM_VOLTAGE_H_
#define UPM_VOLTAGE_H_

#include "upm_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Generic voltage function table */
typedef struct _upm_voltage_ft {
    /* Set sensor offset in volts */
    upm_result_t (*upm_voltage_set_offset) (const void* dev, float offset);
    /* Set sensor scale in volts */
    upm_result_t (*upm_voltage_set_scale) (const void* dev, float scale);
    /* Read sensor value in volts */
    upm_result_t (*upm_voltage_get_value) (const void* dev, float* value);
} upm_voltage_ft;

#ifdef __cplusplus
}
#endif

#endif /* UPM_VOLTAGE_H_ */
