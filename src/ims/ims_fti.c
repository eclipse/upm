/*
 * Author: Noel Eck <noel.eck@intel.com>
 * Copyright (c) 2015 Intel Corporation.
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

#include <string.h>
#include <stdlib.h>

#include "ims.h"
#include "upm_fti.h"
#include "fti/upm_sensor.h"

/** 
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_ims_name[] = "IMS";
const char upm_ims_description[] = "Analog pH Meter Pro";
const upm_protocol_t upm_ims_protocol[] = {UPM_ANALOG};
const upm_sensor_t upm_ims_category[] = {UPM_PH};

// forward declarations
const void* upm_ims_get_ft(upm_sensor_t sensor_type);
void* upm_ims_init_str(const char* protocol, const char* params);
void upm_ims_close(void* dev);
const upm_sensor_descriptor_t upm_ims_get_descriptor();
upm_result_t upm_ims_set_offset(const void* dev, float offset);
upm_result_t upm_ims_set_scale(const void* dev, float scale);
upm_result_t upm_ims_get_value(const void* dev, float *value);

/* This sensor implementes 2 function tables */
/* 1. Generic base function table */
static const upm_sensor_ft ft_gen =
{
    .upm_sensor_init_name = &upm_ims_init_str,
    .upm_sensor_close = &upm_ims_close,
    .upm_sensor_get_descriptor = &upm_ims_get_descriptor
};

/* 2. PH function table */
static const upm_ph_ft ft_ph =
{
    .upm_ph_set_offset = &upm_ims_set_offset,
    .upm_ph_set_scale = &upm_ims_set_scale,
    .upm_ph_get_value = &upm_ims_get_value
};

const void* upm_ims_get_ft(upm_sensor_t sensor_type)
{
    switch(sensor_type)
    {
        case UPM_SENSOR:
            return &ft_gen;
        case UPM_PH:
            return &ft_ph;
        default:
            return NULL;
    }
}

void* upm_ims_init_str(const char* protocol, const char* params)
{
    fprintf(stderr, "String initialization - not implemented, using ain0: %s\n", __FILENAME__);
    return ims_init(0);
}

void upm_ims_close(void* dev)
{
    ims_close(dev);
}

const upm_sensor_descriptor_t upm_ims_get_descriptor()
{
    /* Fill in the descriptor */
    upm_sensor_descriptor_t usd;
    usd.name = upm_ims_name;
    usd.description = upm_ims_description;
    usd.protocol_size = 1;
    usd.protocol = upm_ims_protocol;
    usd.category_size = 1;
    usd.category = upm_ims_category;

    return usd;
}

upm_result_t upm_ims_set_offset(const void* dev, float offset)
{
    return ims_set_offset((ims_context)dev, offset);
}

upm_result_t upm_ims_set_scale(const void* dev, float scale)
{
    return ims_set_scale((ims_context)dev, scale);
}

upm_result_t upm_ims_get_value(const void* dev, float *value)
{
    return ims_get_ph((ims_context)dev, value);
}
