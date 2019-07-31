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
#ifndef UPM_H_
#define UPM_H_

#ifdef __cplusplus
extern "C" {
#endif

#if __STDC_VERSION__ >= 199901L
#define C99
#endif

#include <upm_types.h>
#include <upm_math.h>

#define upm_perror(...) perror(args, __VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* UPM_H_ */
