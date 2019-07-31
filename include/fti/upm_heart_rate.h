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
#ifndef UPM_HEART_RATE_H_
#define UPM_HEART_RATE_H_

#ifdef __cplusplus
extern "C" {
#endif

// Heart rate units
typedef enum _upm_heart_rate_u {BPM} upm_heart_rate_u;

// Heart rate function table
typedef struct _upm_heart_rate_ft {
    upm_result_t (*upm_heart_rate_get_value) (void* dev, int* value, upm_heart_rate_u unit);
} upm_heart_rate_ft;

#ifdef __cplusplus
}
#endif

#endif /* UPM_HEART_RATE_H_ */
