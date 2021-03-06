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

#include <string.h>
#include <stdlib.h>

#include "dfrph.h"
#include "upm_fti.h"
#include "fti/upm_sensor.h"

/** 
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_dfrph_name[] = "DFRPH";
const char upm_dfrph_description[] = "Analog pH Meter Pro";
const upm_protocol_t upm_dfrph_protocol[] = {UPM_ANALOG};
const upm_sensor_t upm_dfrph_category[] = {UPM_PH};

// forward declarations
const void* upm_dfrph_get_ft(upm_sensor_t sensor_type);
void* upm_dfrph_init_str(const char* protocol, const char* params);
void upm_dfrph_close(void* dev);
const upm_sensor_descriptor_t upm_dfrph_get_descriptor();
upm_result_t upm_dfrph_set_offset(const void* dev, float offset);
upm_result_t upm_dfrph_set_scale(const void* dev, float scale);
upm_result_t upm_dfrph_get_value(const void* dev, float *value);

/* This sensor implementes 2 function tables */
/* 1. Generic base function table */
static const upm_sensor_ft ft_gen =
{
    .upm_sensor_init_name = &upm_dfrph_init_str,
    .upm_sensor_close = &upm_dfrph_close,
    .upm_sensor_get_descriptor = &upm_dfrph_get_descriptor
};

/* 2. PH function table */
static const upm_ph_ft ft_ph =
{
    .upm_ph_set_offset = &upm_dfrph_set_offset,
    .upm_ph_set_scale = &upm_dfrph_set_scale,
    .upm_ph_get_value = &upm_dfrph_get_value
};

const void* upm_dfrph_get_ft(upm_sensor_t sensor_type)
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

void* upm_dfrph_init_str(const char* protocol, const char* params)
{
    fprintf(stderr, "String initialization - not implemented, using ain0: %s\n", __FILENAME__);
    return dfrph_init(0);
}

void upm_dfrph_close(void* dev)
{
    dfrph_close(dev);
}

const upm_sensor_descriptor_t upm_dfrph_get_descriptor()
{
    /* Fill in the descriptor */
    upm_sensor_descriptor_t usd;
    usd.name = upm_dfrph_name;
    usd.description = upm_dfrph_description;
    usd.protocol_size = 1;
    usd.protocol = upm_dfrph_protocol;
    usd.category_size = 1;
    usd.category = upm_dfrph_category;

    return usd;
}

upm_result_t upm_dfrph_set_offset(const void* dev, float offset)
{
    return dfrph_set_offset((dfrph_context)dev, offset);
}

upm_result_t upm_dfrph_set_scale(const void* dev, float scale)
{
    return dfrph_set_scale((dfrph_context)dev, scale);
}

upm_result_t upm_dfrph_get_value(const void* dev, float *value)
{
    return dfrph_get_ph((dfrph_context)dev, value);
}
