/*
 * Author:  Sisinty Sasmita Patra <sisinty.s.patra@intel.com>
 *
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include "ldt0028.h"
#include "upm_fti.h"

/** 
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_ldt0028_name[] = "LDT0028";
const char upm_ldt0028_description[] = "Grove Piezo Vibration Sensor";
const upm_protocol_t upm_ldt0028_protocol[] = {UPM_ANALOG};
const upm_sensor_t upm_ldt0028_category[] = {UPM_VIBRATION};

// forward declarations
const void* upm_ldt0028_get_ft(upm_sensor_t sensor_type);
const void* upm_ldt0028_get_ft(upm_sensor_t sensor_type);
const upm_sensor_descriptor_t upm_ldt0028_get_descriptor();
void* upm_ldt0028_init_name();
void upm_ldt0028_close(void* dev);
upm_result_t upm_ldt0028_get_value(void* dev, float* vibval);

const upm_sensor_descriptor_t upm_ldt0028_get_descriptor() {
    upm_sensor_descriptor_t usd;
    usd.name = upm_ldt0028_name;
    usd.description = upm_ldt0028_description;
    usd.protocol_size = 1;
    usd.protocol = upm_ldt0028_protocol;
    usd.category_size = 1;
    usd.category = upm_ldt0028_category;
    return usd;
}

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_ldt0028_init_name,
    .upm_sensor_close = &upm_ldt0028_close,
    .upm_sensor_get_descriptor = &upm_ldt0028_get_descriptor
};

static const upm_vibration_ft vft =
{
    .upm_vibration_get_value = upm_ldt0028_get_value
};

const void* upm_ldt0028_get_ft(upm_sensor_t sensor_type) {
    if(sensor_type == UPM_SENSOR) {
        return &ft;
    }

    if(sensor_type == UPM_VIBRATION) {
        return &vft;
    }
    return NULL;
}

void* upm_ldt0028_init_name(){
    return NULL;
}

void upm_ldt0028_close(void* dev)
{
    ldt0028_close((ldt0028_context)dev);
}

upm_result_t upm_ldt0028_get_value(void* dev, float* vibval)
{
    return ldt0028_get_volts((ldt0028_context)dev, vibval);
}
