/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef UPM_ORP_H_
#define UPM_ORP_H_

#include "upm_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ORP (Oxidation/Reduction Potential) function table */
typedef struct _upm_orp_ft
{
    /* Set sensor offset */
    upm_result_t (*upm_orp_set_offset) (const void* dev, float offset);
    /* Set sensor scale */
    upm_result_t (*upm_orp_set_scale) (const void* dev, float scale);
    /* Read sensor value */
    upm_result_t (*upm_orp_get_value) (const void* dev, float *value);
} upm_orp_ft;

#ifdef __cplusplus
}
#endif

#endif /* UPM_ORP_H_ */
