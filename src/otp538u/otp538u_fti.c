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


#include "otp538u.h"

#include "upm_fti.h"
#include "fti/upm_sensor.h"

/**
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_otp538u_name[] = "OTP538U";
const char upm_otp538u_description[] = "IR Temperature Sensor";
const upm_protocol_t upm_otp538u_protocol[] = {UPM_ANALOG, UPM_ANALOG};
const upm_sensor_t upm_otp538u_category[] = {UPM_TEMPERATURE};

// forward declarations
const upm_sensor_descriptor_t upm_otp538u_get_descriptor();
const void* upm_otp538u_get_ft(upm_sensor_t sensor_type);
upm_result_t upm_otp538u_get_value_temperature(void* dev, float* tempval,
                                               upm_temperature_u unit);
void* upm_otp538u_init_name();
void upm_otp538u_close(void* dev);

const upm_sensor_descriptor_t upm_otp538u_get_descriptor() {
    upm_sensor_descriptor_t usd;
    usd.name = upm_otp538u_name;
    usd.description = upm_otp538u_description;
    usd.protocol_size = 2;
    usd.protocol = upm_otp538u_protocol;
    usd.category_size = 1;
    usd.category = upm_otp538u_category;
    return usd;
}

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_otp538u_init_name,
    .upm_sensor_close = &upm_otp538u_close,
    .upm_sensor_get_descriptor = &upm_otp538u_get_descriptor
};

static const upm_temperature_ft tft =
{
    .upm_temperature_get_value = &upm_otp538u_get_value_temperature
};

const void* upm_otp538u_get_ft(upm_sensor_t sensor_type) {
    if(sensor_type == UPM_SENSOR) {
        return &ft;
    }
    if(sensor_type == UPM_TEMPERATURE) {
        return &tft;
    }
    return NULL;
}

void* upm_otp538u_init_name(){
    return NULL;
}

void upm_otp538u_close(void* dev)
{
    otp538u_close((otp538u_context)dev);
}

upm_result_t upm_otp538u_get_value_temperature(void* dev, float* tempval,
                                               upm_temperature_u unit)
{
    float temp = 0.0;
    upm_result_t rv = otp538u_get_object_temperature((otp538u_context)dev,
                                                     &temp);

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
