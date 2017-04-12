/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * The MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
