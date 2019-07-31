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

#ifndef UPM_VIBRATION_H_
#define UPM_VIBRATION_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _upm_vibration_ft {
    upm_result_t (*upm_vibration_get_value) (void* dev, float* value);
} upm_vibration_ft;

#ifdef __cplusplus
}
#endif

#endif /* UPM_VIBRATION_H_ */
