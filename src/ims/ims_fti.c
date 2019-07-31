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

#include "ims.h"
#include "upm_fti.h"
#include "fti/upm_sensor.h"

/** 
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_ims_name[] = "IMS";
const char upm_ims_description[] = "Catnip Electronics I2C moisture sensor";
const upm_protocol_t upm_ims_protocol[] = {UPM_I2C};
const upm_sensor_t upm_ims_category[] = {UPM_LIGHT, UPM_TEMPERATURE, UPM_MOISTURE};

// forward declarations
const void* upm_ims_get_ft(upm_sensor_t sensor_type);
void* upm_ims_init_str(const char* protocol, const char* params);
void upm_ims_close(void* dev);
const upm_sensor_descriptor_t upm_ims_get_descriptor();
upm_result_t upm_ims_get_light(const void* dev, float *value);
upm_result_t upm_ims_get_temperature(void* dev, float *value, upm_temperature_u unit);
upm_result_t upm_ims_get_moisture(void* dev, int *value);

/* This sensor implementes 2 function tables */
/* 1. Generic base function table */
static const upm_sensor_ft ft_gen =
{
    .upm_sensor_init_name = &upm_ims_init_str,
    .upm_sensor_close = &upm_ims_close,
    .upm_sensor_get_descriptor = &upm_ims_get_descriptor
};

/* 2. Light function table */
static const upm_light_ft ft_light =
{
    .upm_light_set_offset = NULL,
    .upm_light_set_scale = NULL,
    .upm_light_get_value = &upm_ims_get_light
};

/* 3. Light function table */
static const upm_temperature_ft ft_temperature =
{
    .upm_temperature_set_offset = NULL,
    .upm_temperature_set_scale = NULL,
    .upm_temperature_get_value = &upm_ims_get_temperature
};

/* 4. Light function table */
static const upm_moisture_ft ft_moisture =
{
    .upm_moisture_sensor_get_moisture = &upm_ims_get_moisture
};

const void* upm_ims_get_ft(upm_sensor_t sensor_type)
{
    switch(sensor_type)
    {
        case UPM_SENSOR:
            return &ft_gen;
        case UPM_LIGHT:
            return &ft_light;
        case UPM_TEMPERATURE:
            return &ft_temperature;
        case UPM_MOISTURE:
            return &ft_moisture;
        default:
            return NULL;
    }
}

void* upm_ims_init_str(const char* protocol, const char* params)
{
    fprintf(stderr,
            "String initialization - not implemented, using i2c bus 0, addr: 0x20: %s\n", __FILENAME__);
    return ims_init(0, 0x20);
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
    usd.category_size = 3;
    usd.category = upm_ims_category;

    return usd;
}

upm_result_t upm_ims_get_light(const void* dev, float *value)
{
    uint16_t data;
    upm_result_t result = ims_get_light((ims_context*)dev, &data);
    *value = data;
    return result;
}

upm_result_t upm_ims_get_temperature(void* dev, float *value, upm_temperature_u unit)
{
    uint16_t data;
    upm_result_t result = ims_get_temperature((ims_context*)dev, &data);
    *value = data;
    return result;
}

upm_result_t upm_ims_get_moisture(void* dev, int *value)
{
    uint16_t data;
    upm_result_t result = ims_get_moisture((ims_context*)dev, &data);
    *value = data;
    return result;
}
