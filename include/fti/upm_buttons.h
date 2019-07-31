/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
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
