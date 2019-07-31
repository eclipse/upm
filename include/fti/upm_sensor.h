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
#ifndef UPM_SENSOR_H_
#define UPM_SENSOR_H_

#include "upm_types.h"
#include "upm_fti.h"

#ifdef __cplusplus
extern "C" {
#endif

// Generic sensor function table
typedef struct _upm_sensor_ft {
    void* (*upm_sensor_init_name) (const char* protocol, const char* params);
    void (*upm_sensor_close) (void* dev);
    const upm_sensor_descriptor_t (*upm_sensor_get_descriptor) (const void* dev);
} upm_sensor_ft;

#ifdef __cplusplus
}
#endif

#endif /* UPM_SENSOR_H_ */
