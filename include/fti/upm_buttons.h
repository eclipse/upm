/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
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
#ifndef UPM_BUTTONS_H_
#define UPM_BUTTONS_H_

#ifdef __cplusplus
extern "C" {
#endif

// Buttons function table
typedef struct _upm_buttons_ft {
    // This function is used to query button state.  The num_button
    // specifies the total number of buttons present, and the values
    // indicates each button's current value as an array of bools.
    upm_result_t (*upm_buttons_get_num_buttons) (const void *dev,
                                                 unsigned int *num_buttons);
    upm_result_t (*upm_buttons_get_values) (const void *dev,
                                            bool *values);
} upm_buttons_ft;

#ifdef __cplusplus
}
#endif

#endif /* UPM_BUTTONS_H_ */
