/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * Contributions: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 * Contributions: Sarah Knepper <sarah.knepper@intel.com>
 *                Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#include "led.h"
#include "upm_fti.h"

/** 
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_led_name[] = "LED";
const char upm_led_description[] = "LED";
const upm_protocol_t upm_led_protocol[] = {UPM_GPIO};
const upm_sensor_t upm_led_category[] = {UPM_LED};

// forward declarations
const upm_sensor_descriptor_t upm_led_get_descriptor();
const void* upm_led_get_ft(upm_sensor_t sensor_type);
void* upm_led_init_name();
void upm_led_close(void* dev);

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_led_init_name,
    .upm_sensor_close = &upm_led_close,
    .upm_sensor_get_descriptor = &upm_led_get_descriptor
};

const void* upm_led_get_ft(upm_sensor_t sensor_type){
    if(sensor_type == UPM_SENSOR){
        return &ft;
    }
    return NULL;
}

const upm_sensor_descriptor_t upm_led_get_descriptor(){
    upm_sensor_descriptor_t usd;
    usd.name = upm_led_name;
    usd.description = upm_led_description;
    usd.protocol_size = 1;
    usd.protocol = upm_led_protocol;
    usd.category_size = 1;
    usd.category = upm_led_category;
    return usd;
}

void* upm_led_init_name(){
    return NULL;
}

void upm_led_close(void* dev) {
    led_close((led_context)dev);
}
