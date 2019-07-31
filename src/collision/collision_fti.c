/*
 * Author: Zion Orent <sorent@ics.com>
 *         Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#include "collision.h"
#include "upm_fti.h"

/** 
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_collision_name[] = "Collision";
const char upm_collision_description[] = "Collision sensor";
const upm_protocol_t upm_collision_protocol[] = {UPM_GPIO};
const upm_sensor_t upm_collision_category[] = {UPM_ELECTRICITY};

// forward declarations
const upm_sensor_descriptor_t upm_collision_get_descriptor();
const void* upm_collision_get_ft(upm_sensor_t sensor_type);
void* upm_collision_init_name();
void upm_collision_close(void* dev);

const upm_sensor_descriptor_t upm_collision_get_descriptor(){
    upm_sensor_descriptor_t usd;
    usd.name = upm_collision_name;
    usd.description = upm_collision_description;
    usd.protocol_size = 1;
    usd.protocol = upm_collision_protocol;
    usd.category_size = 1;
    usd.category = upm_collision_category;
    return usd;
}

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_collision_init_name,
    .upm_sensor_close = &upm_collision_close,
    .upm_sensor_get_descriptor = &upm_collision_get_descriptor
};

const void* upm_collision_get_ft(upm_sensor_t sensor_type){
    if(sensor_type == UPM_SENSOR){
        return &ft;
    }
    return NULL;
}

void* upm_collision_init_name(){
    return NULL;
}

void upm_collision_close(void* dev){
    collision_close((collision_context)dev);
}

upm_result_t upm_collision_is_colliding(void* dev, bool* collision_val){
    return collision_is_colliding((collision_context)dev, collision_val);
}