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
#ifndef UPM_SWITCH_H_
#define UPM_SWITCH_H_

#ifdef __cplusplus
extern "C" {
#endif

// Switch function table
typedef struct _upm_switch_ft {
    upm_result_t (*upm_switch_get_value) (void* dev, bool* value, int num);
    upm_result_t (*upm_switch_attach_isr) (void* dev, void (*isr)(void *), void *arg);
    upm_result_t (*upm_switch_clear_isr) (void* dev);
} upm_switch_ft;

#ifdef __cplusplus
}
#endif

#endif /* UPM_SWITCH_H_ */
