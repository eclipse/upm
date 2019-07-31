/*
 * Author: Sarah Knepper <sarah.knepper@intel.com>
 *         Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include "ttp223.h"
#include "upm_fti.h"

/** 
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_ttp223_name[] = "TTP223";
const char upm_ttp223_description[] = "TTP223 Touch Detector Sensor";
const upm_protocol_t upm_ttp223_protocol[] = {UPM_GPIO};
const upm_sensor_t upm_ttp223_category[] = {UPM_SWITCH};

// forward declarations
const upm_sensor_descriptor_t upm_ttp223_get_descriptor();
const void* upm_ttp223_get_ft(upm_sensor_t sensor_type);
void* upm_ttp223_init_name();
void upm_ttp223_close(void* dev);
upm_result_t upm_ttp223_is_pressed(void* dev, bool* value, int num);

const upm_sensor_descriptor_t upm_ttp223_get_descriptor(){
    upm_sensor_descriptor_t usd;
    usd.name = upm_ttp223_name;
    usd.description = upm_ttp223_description;
    usd.protocol_size = 1;
    usd.protocol = upm_ttp223_protocol;
    usd.category_size = 1;
    usd.category = upm_ttp223_category;
    return usd;
}

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_ttp223_init_name,
    .upm_sensor_close = &upm_ttp223_close,
    .upm_sensor_get_descriptor = &upm_ttp223_get_descriptor
};

static const upm_switch_ft sft =
{
    .upm_switch_get_value = &upm_ttp223_is_pressed
};

const void* upm_ttp223_get_ft(upm_sensor_t sensor_type){
    if(sensor_type == UPM_SWITCH){
        return &sft;
    }
    else if(sensor_type == UPM_SENSOR){
        return &ft;
    }
    return NULL;
}

void* upm_ttp223_init_name(){
    return NULL;
}

void upm_ttp223_close(void* dev){
    ttp223_close((ttp223_context)dev);
}

upm_result_t upm_ttp223_is_pressed(void* dev, bool* value, int num) {
    if(num != 1){
        printf("Sorry this sensor driver supports only one touch pad\n");
        return UPM_ERROR_OUT_OF_RANGE;
    }

    return ttp223_is_pressed((ttp223_context)dev, value);
}