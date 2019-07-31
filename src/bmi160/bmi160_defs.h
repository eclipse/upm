/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * The MIT License
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define BMI160_CHIP_ID 0xd1

    typedef enum {
        BMI160_ACC_RANGE_2G                        = 0, // 2 Gravities
        BMI160_ACC_RANGE_4G,
        BMI160_ACC_RANGE_8G,
        BMI160_ACC_RANGE_16G
    } BMI160_ACC_RANGE_T;

    typedef enum {
        BMI160_GYRO_RANGE_125                      = 0, // 125 degrees/sec
        BMI160_GYRO_RANGE_250,
        BMI160_GYRO_RANGE_500,
        BMI160_GYRO_RANGE_1000,
        BMI160_GYRO_RANGE_2000
    } BMI160_GYRO_RANGE_T;

#ifdef __cplusplus
}
#endif
