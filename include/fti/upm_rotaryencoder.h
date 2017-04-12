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
