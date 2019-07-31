/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef UPM_ROTARYENCODER_H_
#define UPM_ROTARYENCODER_H_

#include "upm_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Rotary Encoder function table */
typedef struct _upm_rotaryencoder_ft
{
    /* Set the initial position (count) */
    upm_result_t (*upm_rotaryencoder_set_position) (const void* dev, int pos);
    /* Get the current position (count) */
    upm_result_t (*upm_rotaryencoder_get_position) (const void* dev, int *pos);
} upm_rotaryencoder_ft;

#ifdef __cplusplus
}
#endif

#endif /* UPM_ROTARYENCODER_H_ */
