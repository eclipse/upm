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
#include "loudness.h"
#include "upm_fti.h"

/**
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_loudness_name[] = "loudness";
const char upm_loudness_description[] = " Loudness Sensor";
const upm_protocol_t upm_loudness_protocol[] = {UPM_ANALOG};
const upm_sensor_t upm_loudness_category[] = {UPM_AUDIO};

// forward declarations
const void* upm_loudness_get_ft(upm_sensor_t sensor_type);
const upm_sensor_descriptor_t upm_loudness_get_descriptor();
void* upm_loudness_init_name();
void upm_loudness_close(void* dev);


const upm_sensor_descriptor_t upm_loudness_get_descriptor(){
    upm_sensor_descriptor_t usd;
    usd.name = upm_loudness_name;
    usd.description = upm_loudness_description;
    usd.protocol_size = 1;
    usd.protocol = upm_loudness_protocol;
    usd.category_size = 1;
    usd.category = upm_loudness_category;
    return usd;
}

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_loudness_init_name,
    .upm_sensor_close = &upm_loudness_close,
    .upm_sensor_get_descriptor = &upm_loudness_get_descriptor
};

const void* upm_loudness_get_ft(upm_sensor_t sensor_type){
    if(sensor_type == UPM_SENSOR){
        return &ft;
    }
    return NULL;
}

void* upm_loudness_init_name(){
    return NULL;
}

void upm_loudness_close(void* dev){
    loudness_close((loudness_context)dev);
}

upm_result_t upm_loudness_get_value(void* dev, int* val){
    return loudness_get_value((loudness_context)dev, val);
}