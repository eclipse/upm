/*
 * Author: Jon Trulson <jtrulson@ics.com>
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
#ifndef UPM_EC_H_
#define UPM_EC_H_

#include "upm_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* EC (Electrical Conductivity) function table */
typedef struct _upm_ec_ft
{
    /* Set sensor offset */
    upm_result_t (*upm_ec_set_offset) (const void* dev, float offset);
    /* Set sensor scale */
    upm_result_t (*upm_ec_set_scale) (const void* dev, float scale);
    /* Read sensor value */
    upm_result_t (*upm_ec_get_value) (const void* dev, float *value);
} upm_ec_ft;

#ifdef __cplusplus
}
#endif

#endif /* UPM_EC_H_ */
