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
#ifndef UPM_SERVO_H_
#define UPM_SERVO_H_

#ifdef __cplusplus
extern "C" {
#endif

// Servo function table
typedef struct _upm_servo_ft {
    upm_result_t (*upm_servo_set_angle) (void* dev, int angle);
} upm_servo_ft;

#ifdef __cplusplus
}
#endif

#endif /* UPM_SERVO_H_ */

