/*
 * Authors:
 *          Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef UPM_MATH_H_
#define UPM_MATH_H_

#include <upm_platform.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(UPM_PLATFORM_LINUX) || defined(UPM_PLATFORM_ZEPHYR)
#include <math.h>
#endif

#ifndef M_PI
#define M_PI           (3.14159265358979323846)
#endif

#ifdef __cplusplus
}
#endif

#endif /* UPM_MATH_H_ */
