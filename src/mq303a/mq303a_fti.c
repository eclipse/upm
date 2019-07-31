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
#include "mq303a.h"
#include "upm_fti.h"

/** 
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_mq303a_name[] = "MQ303A";
const char upm_mq303a_description[] = "MQ303A";
const upm_protocol_t upm_mq303a_protocol[] = {UPM_GPIO, UPM_ANALOG};
const upm_sensor_t upm_mq303a_category[] = {UPM_GAS};

// forward declarations
const upm_sensor_descriptor_t upm_mq303a_get_descriptor();
const void* upm_mq303a_get_ft(upm_sensor_t sensor_type);
void* upm_mq303a_init_name();
void upm_mq303a_close(void* dev);
upm_result_t upm_mq303a_get_value(void* dev, int* val);

const upm_sensor_descriptor_t upm_mq303a_get_descriptor(){
    upm_sensor_descriptor_t usd;
    usd.name = upm_mq303a_name;
    usd.description = upm_mq303a_description;
    usd.protocol_size = 1;
    usd.protocol = upm_mq303a_protocol;
    usd.category_size = 1;
    usd.category = upm_mq303a_category;
    return usd;
}

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_mq303a_init_name,
    .upm_sensor_close = &upm_mq303a_close,
    .upm_sensor_get_descriptor = &upm_mq303a_get_descriptor
};

const void* upm_mq303a_get_ft(upm_sensor_t sensor_type){
    if (sensor_type == UPM_SENSOR){
        return &ft;
    }
    return NULL;
}

void* upm_mq303a_init_name(){
    return NULL;
}

void upm_mq303a_close(void* dev){
    mq303a_close((mq303a_context)dev);
}

upm_result_t upm_mq303a_get_value(void* dev, int* val){
    return mq303a_get_value((mq303a_context)dev, val);
}