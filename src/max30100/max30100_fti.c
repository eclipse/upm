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

#include "max30100.h"
#include "upm_fti.h"
#include "fti/upm_sensor.h"

/** 
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_max30100_name[] = "MAX30100";
const char upm_max30100_description[] = "Pulse oximeter and heart-rate sensor";
const upm_protocol_t upm_max30100_protocol[] = {UPM_I2C};
/* TODO: Add/implement heart rate and SpO2 categories */
const upm_sensor_t upm_max30100_category[] = {UPM_TEMPERATURE};

// forward declarations
const void* upm_max30100_get_ft(upm_sensor_t sensor_type);
void* upm_max30100_init_str(const char* protocol, const char* params);
void upm_max30100_close(void* dev);
const upm_sensor_descriptor_t upm_max30100_get_descriptor();
upm_result_t upm_max30100_get_temperature(void* dev, float *value, upm_temperature_u unit);

/* This sensor implementes 2 function tables */
/* 1. Generic base function table */
static const upm_sensor_ft ft_gen =
{
    .upm_sensor_init_name = &upm_max30100_init_str,
    .upm_sensor_close = &upm_max30100_close,
    .upm_sensor_get_descriptor = &upm_max30100_get_descriptor
};

/* 2. Temperatur function table */
static const upm_temperature_ft ft_temperature =
{
    .upm_temperature_set_offset = NULL,
    .upm_temperature_set_scale = NULL,
    .upm_temperature_get_value = &upm_max30100_get_temperature
};

const void* upm_max30100_get_ft(upm_sensor_t sensor_type)
{
    switch(sensor_type)
    {
        case UPM_SENSOR:
            return &ft_gen;
        case UPM_TEMPERATURE:
            return &ft_temperature;
        default:
            return NULL;
    }
}

void* upm_max30100_init_str(const char* protocol, const char* params)
{
    fprintf(stderr,
            "String initialization - not implemented, using i2c bus 0: %s\n", __FILENAME__);
    return max30100_init(0);
}

void upm_max30100_close(void* dev)
{
    max30100_close(dev);
}

const upm_sensor_descriptor_t upm_max30100_get_descriptor()
{
    /* Fill in the descriptor */
    upm_sensor_descriptor_t usd;
    usd.name = upm_max30100_name;
    usd.description = upm_max30100_description;
    usd.protocol_size = 1;
    usd.protocol = upm_max30100_protocol;
    usd.category_size = 2;
    usd.category = upm_max30100_category;

    return usd;
}

upm_result_t upm_max30100_get_temperature(void* dev, float *value, upm_temperature_u unit)
{
    upm_result_t result = max30100_get_temperature((max30100_context*)dev, value);
    return result;
}
