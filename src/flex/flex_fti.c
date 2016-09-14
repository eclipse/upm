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

#include "flex.h"
#include "upm_fti.h"
#include "fti/upm_sensor.h"
#include "fti/upm_raw.h"

/** 
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_flex_name[] = "flex";
const char upm_flex_description[] = "Analog flex sensor";
const upm_protocol_t upm_flex_protocol[] = {UPM_ANALOG};
const upm_sensor_t upm_flex_category[] = {UPM_RAW};

// forward declarations
const void* upm_flex_get_ft(upm_sensor_t sensor_type);
void* upm_flex_init_str(const char* protocol, const char* params);
void upm_flex_close(void* dev);
const upm_sensor_descriptor_t upm_flex_get_descriptor();
upm_result_t upm_flex_set_offset(const void* dev, float offset);
upm_result_t upm_flex_set_scale(const void* dev, float scale);
upm_result_t upm_flex_get_value(const void* dev, float *value);

/* This sensor implementes 2 function tables */
/* 1. Generic base function table */
static const upm_sensor_ft ft_gen =
{
    .upm_sensor_init_name = &upm_flex_init_str,
    .upm_sensor_close = &upm_flex_close,
    .upm_sensor_get_descriptor = &upm_flex_get_descriptor
};

/* 2. RAW function table */
static const upm_raw_ft ft_raw =
{
    .upm_raw_set_offset = &upm_flex_set_offset,
    .upm_raw_set_scale = &upm_flex_set_scale,
    .upm_raw_get_value = &upm_flex_get_value
};

const void* upm_flex_get_ft(upm_sensor_t sensor_type)
{
    switch(sensor_type)
    {
        case UPM_SENSOR:
            return &ft_gen;
        case UPM_RAW:
            return &ft_raw;
        default:
            return NULL;
    }
}

void* upm_flex_init_str(const char* protocol, const char* params)
{
    fprintf(stderr, "String initialization - not implemented, using ain0: %s\n", __FILENAME__);
    return flex_init(0);
}

void upm_flex_close(void* dev)
{
    flex_close((flex_context)dev);
}

const upm_sensor_descriptor_t upm_flex_get_descriptor()
{
    /* Fill in the descriptor */
    upm_sensor_descriptor_t usd;
    usd.name = upm_flex_name;
    usd.description = upm_flex_description;
    usd.protocol_size = 1;
    usd.protocol = upm_flex_protocol;
    usd.category_size = 1;
    usd.category = upm_flex_category;

    return usd;
}

upm_result_t upm_flex_set_offset(const void* dev, float offset)
{
    return flex_set_offset((flex_context)dev, offset);
}

upm_result_t upm_flex_set_scale(const void* dev, float scale)
{
    return flex_set_scale((flex_context)dev, scale);
}

upm_result_t upm_flex_get_value(const void* dev, float *value)
{
    return flex_get_volts((flex_context)dev, value);
}
