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

#include "moisture.h"
#include "upm_fti.h"

/**
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_moisture_name[] = "Moisture";
const char upm_moisture_description[] = "Analog Moisture Sensor";
const upm_protocol_t upm_moisture_protocol[] = {UPM_ANALOG};
const upm_sensor_t upm_moisture_category[] = {UPM_MOISTURE};

// forward declarations
const void* upm_moisture_get_ft(upm_sensor_t sensor_type);
void* upm_moisture_init_name();
void upm_moisture_close(void* dev);
upm_result_t upm_moisture_get_moisture(void* dev, int* moisture);


const upm_sensor_descriptor_t upm_moisture_get_descriptor (){
    upm_sensor_descriptor_t usd;
    usd.name = upm_moisture_name;
    usd.description = upm_moisture_description;
    usd.protocol_size = 1;
    usd.protocol = upm_moisture_protocol;
    usd.category_size = 1;
    usd.category = upm_moisture_category;
    return usd;
}

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_moisture_init_name,
    .upm_sensor_close = &upm_moisture_close,
    .upm_sensor_get_descriptor = &upm_moisture_get_descriptor
};

static const upm_moisture_ft mft =
{
    .upm_moisture_sensor_get_moisture = &upm_moisture_get_moisture
};

const void* upm_moisture_get_ft(upm_sensor_t sensor_type){
    if(sensor_type == UPM_MOISTURE){
        return &mft;
    }
    if(sensor_type == UPM_SENSOR){
        return &ft;
    }
    return NULL;
}

void* upm_moisture_init_name(){
    return NULL;
}

void upm_moisture_close(void* dev){
    moisture_close((moisture_context)dev);
}

upm_result_t upm_moisture_get_moisture(void* dev, int* moisture){
    return moisture_get_moisture((moisture_context)dev, moisture);
}
