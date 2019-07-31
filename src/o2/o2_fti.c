/*
 * Author: Noel Eck <noel.eck@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include "o2.h"
#include "upm_fti.h"
#include "fti/upm_sensor.h"
#include "fti/upm_raw.h"

/** 
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_o2_name[] = "O2";
const char upm_o2_description[] = "Analog oxygen sensor";
const upm_protocol_t upm_o2_protocol[] = {UPM_ANALOG};
const upm_sensor_t upm_o2_category[] = {UPM_RAW};

// forward declarations
const void* upm_o2_get_ft(upm_sensor_t sensor_type);
void* upm_o2_init_str(const char* protocol, const char* params);
void upm_o2_close(void* dev);
const upm_sensor_descriptor_t upm_o2_get_descriptor();
upm_result_t upm_o2_set_offset(const void* dev, float offset);
upm_result_t upm_o2_set_scale(const void* dev, float scale);
upm_result_t upm_o2_get_value(const void* dev, float *value);

/* This sensor implementes 2 function tables */
/* 1. Generic base function table */
static const upm_sensor_ft ft_gen =
{
    .upm_sensor_init_name = &upm_o2_init_str,
    .upm_sensor_close = &upm_o2_close,
    .upm_sensor_get_descriptor = &upm_o2_get_descriptor
};

/* 2. RAW function table */
static const upm_raw_ft ft_raw =
{
    .upm_raw_set_offset = &upm_o2_set_offset,
    .upm_raw_set_scale = &upm_o2_set_scale,
    .upm_raw_get_value = &upm_o2_get_value
};

const void* upm_o2_get_ft(upm_sensor_t sensor_type)
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

void* upm_o2_init_str(const char* protocol, const char* params)
{
    fprintf(stderr, "String initialization - not implemented, using ain0: %s\n", __FILE__);
    return o2_init(0);
}

void upm_o2_close(void* dev)
{
    o2_close((o2_context)dev);
}

const upm_sensor_descriptor_t upm_o2_get_descriptor()
{
    /* Fill in the descriptor */
    upm_sensor_descriptor_t usd;
    usd.name = upm_o2_name;
    usd.description = upm_o2_description;
    usd.protocol_size = 1;
    usd.protocol = upm_o2_protocol;
    usd.category_size = 1;
    usd.category = upm_o2_category;

    return usd;
}

upm_result_t upm_o2_set_offset(const void* dev, float offset)
{
    return o2_set_offset((o2_context)dev, offset);
}

upm_result_t upm_o2_set_scale(const void* dev, float scale)
{
    return o2_set_scale((o2_context)dev, scale);
}

upm_result_t upm_o2_get_value(const void* dev, float *value)
{
    return o2_get_value((o2_context)dev, value);
}
