/*
 * Author: Sisinty Sasmita Patra <sisinty.s.patra@intel.com>
 *
 * Copyright (c) 2016 Intel Corporation.
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


#include<stdio.h>
#include <stdlib.h>

#include "temperature.h"
#include "upm_fti.h"
#include "upm_sensor.h"

/** 
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_temperature_name[] = "Temperature Sensor";
const char upm_temperature_description[] = "Analog Temperature Sensor";
const upm_protocol_t upm_temperature_protocol[] = {UPM_ANALOG};
const upm_sensor_t upm_temperature_category[] = {UPM_TEMPERATURE};

// forward declarations
const upm_sensor_descriptor_t upm_temperature_get_descriptor();
const void* upm_temperature_get_ft(upm_sensor_t sensor_type);
upm_result_t upm_temperature_get_value_temperature(void* dev, float* tempval,
                                                   upm_temperature_u unit);
void* upm_temperature_init_name();
void upm_temperature_close(void* dev);

const upm_sensor_descriptor_t upm_temperature_get_descriptor() {
    upm_sensor_descriptor_t usd;
    usd.name = upm_temperature_name;
    usd.description = upm_temperature_description;
    usd.protocol_size = 1;
    usd.protocol = upm_temperature_protocol;
    usd.category_size = 1;
    usd.category = upm_temperature_category;
    return usd;
}

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_temperature_init_name,
    .upm_sensor_close = &upm_temperature_close,
    .upm_sensor_get_descriptor = &upm_temperature_get_descriptor
};

static const upm_temperature_ft tft =
{
    .upm_temperature_get_value = &upm_temperature_get_value_temperature
};

const void* upm_temperature_get_ft(upm_sensor_t sensor_type) {
    if(sensor_type == UPM_SENSOR) {
        return &ft;
    }
    if(sensor_type == UPM_TEMPERATURE) {
        return &tft;
    }
    return NULL;
}

void* upm_temperature_init_name(){
    return NULL;
}

void upm_temperature_close(void* dev)
{
    temperature_close((temperature_context)dev);
}

upm_result_t upm_temperature_get_value_temperature(void* dev, float* tempval,
                                                   upm_temperature_u unit)
{
    float temp = 0.0;
    upm_result_t rv = temperature_get_value((temperature_context)dev, &temp);

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
