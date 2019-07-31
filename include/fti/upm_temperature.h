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
#ifndef UPM_TEMPERATURE_H_
#define UPM_TEMPERATURE_H_

#ifdef __cplusplus
extern "C" {
#endif

// Temperature units
typedef enum _upm_temperature_u {CELSIUS, FAHRENHEIT, KELVIN} upm_temperature_u;

// Temperature function table
typedef struct _upm_temperature_ft {
    upm_result_t (*upm_temperature_set_scale) (void* dev, float scale);
    upm_result_t (*upm_temperature_set_offset) (void* dev, float offset);
    upm_result_t (*upm_temperature_get_value) (void* dev, float* value, upm_temperature_u unit);
} upm_temperature_ft;

#ifdef __cplusplus
}
#endif

#endif /* UPM_TEMPERATURE_H_ */
