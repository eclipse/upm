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

#include <string.h>
#include <stdlib.h>

#include "guvas12d.h"
#include "fti/upm_sensor.h"
#include "fti/upm_voltage.h"

/**
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_guvas12d_name[] = "GUVAS12D";
const char upm_guvas12d_description[] = "Analog UV sensor";
const upm_protocol_t upm_guvas12d_protocol[] = {UPM_ANALOG};
const upm_sensor_t upm_guvas12d_category[] = {UPM_VOLTAGE};

// forward declarations
void* upm_guvas12d_init_str(const char* protocol, const char* params);
void upm_guvas12d_close(void* dev);
const void* upm_guvas12d_get_ft(upm_sensor_t sensor_type);
const upm_sensor_descriptor_t upm_guvas12d_get_descriptor();
upm_result_t upm_guvas12d_set_offset(const void* dev, float offset);
upm_result_t upm_guvas12d_set_scale(const void* dev, float scale);
upm_result_t upm_guvas12d_get_value(const void* dev, float *value);

/* This sensor implementes 2 function tables */
/* 1. Generic base function table */
static const upm_sensor_ft ft_gen =
{
    .upm_sensor_init_name = &upm_guvas12d_init_str,
    .upm_sensor_close = &upm_guvas12d_close,
    .upm_sensor_get_descriptor = &upm_guvas12d_get_descriptor
};

/* 2. VOLTAGE function table */
static const upm_voltage_ft ft_voltage =
{
    .upm_voltage_set_offset = &upm_guvas12d_set_offset,
    .upm_voltage_set_scale = &upm_guvas12d_set_scale,
    .upm_voltage_get_value = &upm_guvas12d_get_value
};

const void* upm_guvas12d_get_ft(upm_sensor_t sensor_type)
{
    switch(sensor_type)
    {
        case UPM_SENSOR:
            return &ft_gen;
        case UPM_VOLTAGE:
            return &ft_voltage;
        default:
            return NULL;
    }
}

void* upm_guvas12d_init_str(const char* protocol, const char* params)
{
    return NULL;
}

void upm_guvas12d_close(void* dev)
{
    guvas12d_close((guvas12d_context)dev);
}

const upm_sensor_descriptor_t upm_guvas12d_get_descriptor()
{
    /* Fill in the descriptor */
    upm_sensor_descriptor_t usd;
    usd.name = upm_guvas12d_name;
    usd.description = upm_guvas12d_description;
    usd.protocol_size = 1;
    usd.protocol = upm_guvas12d_protocol;
    usd.category_size = 1;
    usd.category = upm_guvas12d_category;

    return usd;
}

upm_result_t upm_guvas12d_set_offset(const void* dev, float offset)
{
    guvas12d_set_offset((guvas12d_context)dev, offset);
    return UPM_SUCCESS;
}

upm_result_t upm_guvas12d_set_scale(const void* dev, float scale)
{
    guvas12d_set_scale((guvas12d_context)dev, scale);
    return UPM_SUCCESS;
}

upm_result_t upm_guvas12d_get_value(const void* dev, float *value)
{
    return guvas12d_get_volts((guvas12d_context)dev, value);
}
