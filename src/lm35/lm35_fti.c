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

#include "lm35.h"

#include <stdio.h>
#include <stdlib.h>

#include "upm_fti.h"
#include "fti/upm_sensor.h"

/**
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_lm35_name[] = "LM35";
const char upm_lm35_description[] = "LM35 Analog Temperature Sensor";
const upm_protocol_t upm_lm35_protocol[] = {UPM_ANALOG};
const upm_sensor_t upm_lm35_category[] = {UPM_TEMPERATURE};

// forward declarations
const upm_sensor_descriptor_t upm_lm35_get_descriptor();
const void* upm_lm35_get_ft(upm_sensor_t sensor_type);
upm_result_t upm_lm35_get_value(void* dev, float* tempval,
                                upm_temperature_u unit);
void* upm_lm35_init_name();
void upm_lm35_close(void* dev);
upm_result_t upm_lm35_set_offset(void *dev, float offset);
upm_result_t upm_lm35_set_scale(void *dev, float scale);

const upm_sensor_descriptor_t upm_lm35_get_descriptor()
{
    upm_sensor_descriptor_t usd;
    usd.name = upm_lm35_name;
    usd.description = upm_lm35_description;
    usd.protocol_size = 1;
    usd.protocol = upm_lm35_protocol;
    usd.category_size = 1;
    usd.category = upm_lm35_category;
    return usd;
}

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_lm35_init_name,
    .upm_sensor_close = &upm_lm35_close,
    .upm_sensor_get_descriptor = &upm_lm35_get_descriptor
};

static const upm_temperature_ft tft =
{
    .upm_temperature_get_value = &upm_lm35_get_value,
    .upm_temperature_set_scale = &upm_lm35_set_scale,
    .upm_temperature_set_offset = &upm_lm35_set_offset,
};

const void* upm_lm35_get_ft(upm_sensor_t sensor_type)
{
    if (sensor_type == UPM_SENSOR)
        return &ft;

    if (sensor_type == UPM_TEMPERATURE)
        return &tft;

    return NULL;
}

void* upm_lm35_init_name()
{
    return NULL;
}

void upm_lm35_close(void* dev)
{
    lm35_close((lm35_context)dev);
}

upm_result_t upm_lm35_set_offset(void *dev, float offset)
{
    lm35_set_offset((lm35_context)dev, offset);
    return UPM_SUCCESS;
}
upm_result_t upm_lm35_set_scale(void *dev, float scale)
{
    lm35_set_scale((lm35_context)dev, scale);
    return UPM_SUCCESS;
}

upm_result_t upm_lm35_get_value(void* dev, float* tempval,
                                 upm_temperature_u unit)
{
    float temp = 0.0;
    upm_result_t rv = lm35_get_temperature((lm35_context)dev, &temp);

    if (rv != UPM_SUCCESS)
        return rv;

    switch (unit)
        {
        case CELSIUS:
            *tempval = temp;
            return UPM_SUCCESS;

        case KELVIN:
            *tempval = temp + 273.15;
            return UPM_SUCCESS;

        case FAHRENHEIT:
            *tempval = temp * (9.0/5.0) + 32.0;
            return UPM_SUCCESS;
        }

    return UPM_ERROR_INVALID_PARAMETER;
}
