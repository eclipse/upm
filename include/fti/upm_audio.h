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

#ifndef UPM_AUDIO_H_
#define UPM_AUDIO_H_

#ifdef __cplusplus
extern "C" {
#endif

// Audio unit
typedef enum _upm_audio_u {DECIBELS} upm_audio_u;

// Audio function table
typedef struct _upm_audio_ft {
    upm_result_t (*upm_audio_get_value) (void* dev, float* value, upm_audio_u unit);
} upm_audio_ft;

#ifdef __cplusplus
}
#endif

#endif /* UPM_AUDIO_H_ */
