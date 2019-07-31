/*
 * Author: Jon Trulson <jtrulson@ics.com>
 *         Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include "mpr121.h"
#include "upm_fti.h"

/** 
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_mpr121_name[] = "MPR121";
const char upm_mpr121_description[] = "MPR121 Touch Sensor";
const upm_protocol_t upm_mpr121_protocol[] = {UPM_I2C};
const upm_sensor_t upm_mpr121_category[] = {UPM_SWITCH};

// forward declarationss
const upm_sensor_descriptor_t upm_mpr121_get_descriptor ();
const void* upm_mpr121_get_ft(upm_sensor_t sensor_type);
void* upm_mpr121_init_name();
void upm_mpr121_close(void* dev);

const upm_sensor_descriptor_t upm_mpr121_get_descriptor (){
    upm_sensor_descriptor_t usd;
    usd.name = upm_mpr121_name;
    usd.description = upm_mpr121_description;
    usd.protocol_size = 1;
    usd.protocol = upm_mpr121_protocol;
    usd.category_size = 1;
    usd.category = upm_mpr121_category;
    return usd;
}

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_mpr121_init_name,
    .upm_sensor_close = &upm_mpr121_close,
    .upm_sensor_get_descriptor = &upm_mpr121_get_descriptor
};

const void* upm_mpr121_get_ft(upm_sensor_t sensor_type){
    if(sensor_type == UPM_SENSOR){
        return &ft;
    }
    return NULL;
}

void* upm_mpr121_init_name(){
    return NULL;
}

void upm_mpr121_close(void* dev){
    mpr121_close((mpr121_context)dev);
}
