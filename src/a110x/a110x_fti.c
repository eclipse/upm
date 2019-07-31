/*
 * Author: Jon Trulson <jtrulson@ics.com>
 *         Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#include "a110x.h"
#include "upm_fti.h"

/**
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_a110x_name[] = "A110X";
const char upm_a110x_description[] = "A110X Hall Effect Sensor";
const upm_protocol_t upm_a110x_protocol[] = {UPM_GPIO};
const upm_sensor_t upm_a110x_category[] = {UPM_ELECTRICITY};

// forward declarations
const upm_sensor_descriptor_t upm_a110x_get_descriptor();
const void* upm_a110x_get_ft(upm_sensor_t sensor_type);
void* upm_a110x_init_name();
void upm_a110x_close(void* dev);

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_a110x_init_name,
    .upm_sensor_close = &upm_a110x_close,
    .upm_sensor_get_descriptor = &upm_a110x_get_descriptor
};

const void* upm_a110x_get_ft(upm_sensor_t sensor_type){
    if (sensor_type == UPM_SENSOR){
        return &ft;
    }
    return NULL;
}

const upm_sensor_descriptor_t upm_a110x_get_descriptor(){
    upm_sensor_descriptor_t usd;
    usd.name = upm_a110x_name;
    usd.description = upm_a110x_description;
    usd.protocol_size = 1;
    usd.protocol = upm_a110x_protocol;
    usd.category_size = 1;
    usd.category = upm_a110x_category;
    return usd;
}

void* upm_a110x_init_name(){
    return NULL;
}

void upm_a110x_close(void* dev){
    a110x_close((a110x_context)dev);
}
