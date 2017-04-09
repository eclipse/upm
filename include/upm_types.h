/*
 * Authors:
 * Copyright (c) 2016 Intel Corporation.
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
#ifndef UPM_TYPES_H_
#define UPM_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef C99
#include <stdbool.h>
#elif __cplusplus
#else
typedef enum {
    false = 0,
    true = 1
} bool;
#endif

typedef enum {
    UPM_SUCCESS = 0,                        /* Operation is successful, expected response */
    UPM_ERROR_NOT_IMPLEMENTED = 1,          /* Trying to access a feature or mode that is not implemented */
    UPM_ERROR_NOT_SUPPORTED = 2,            /* Trying to access a feature or mode that is not supported */
    UPM_ERROR_NO_RESOURCES = 3,             /* No resources to perform operation */
    UPM_ERROR_NO_DATA = 4,                  /* No data received or available from the sensor */
    UPM_ERROR_INVALID_PARAMETER = 5,        /* Invalid parameter passed to the function*/
    UPM_ERROR_INVALID_SIZE = 6,             /* Invalid buffer size */
    UPM_ERROR_OUT_OF_RANGE = 7,             /* When the input to drive is too high/low or -ve */
    UPM_ERROR_OPERATION_FAILED = 8,         /* When a function isn't able to perform as expected */
    UPM_ERROR_TIMED_OUT = 9,                /* Timed out while communicating with the sensor */

    UPM_ERROR_UNSPECIFIED = 99              /* Unspecified/Unknown error */
} upm_result_t;

#ifdef __cplusplus
}
#endif

#endif /* UPM_TYPES_H_ */
