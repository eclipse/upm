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

#ifndef UPM_SENSOR_MOISTURE_H_
#define UPM_SENSOR_MOISTURE_H_
#include <stdarg.h>

typedef struct _upm_moisture_ft {
    upm_result_t (*upm_moisture_sensor_get_moisture) (void* dev, int* moisture);
} upm_moisture_ft;

#endif /* UPM_SENSOR_MOISTURE_H_ */
