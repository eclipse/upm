/*
 * Author: Sisinty Sasmita Patra <sisinty.s.patra@intel.com>
 *
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include "relay.h"
#include "upm_fti.h"

/** 
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_relay_name[] = "Grove Relay";
const char upm_relay_description[] = "Grove relay Sensor";
const upm_protocol_t upm_relay_protocol[] = {UPM_GPIO};
const upm_sensor_t upm_relay_category[] = {UPM_SWITCH};

// forward declarations
const upm_sensor_descriptor_t upm_relay_get_descriptor();
const void* upm_relay_get_ft(upm_sensor_t sensor_type);
void* upm_relay_init_name();
void upm_relay_close(void* dev);
upm_result_t upm_relay_get_value(void* dev, bool* value, int num);

const upm_sensor_descriptor_t upm_relay_get_descriptor() {
    upm_sensor_descriptor_t usd;
    usd.name = upm_relay_name;
    usd.description = upm_relay_description;
    usd.protocol_size = 1;
    usd.protocol = upm_relay_protocol;
    usd.category_size = 1;
    usd.category = upm_relay_category;
    return usd;
}

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_relay_init_name,
    .upm_sensor_close = &upm_relay_close,
    .upm_sensor_get_descriptor = &upm_relay_get_descriptor
};

static const upm_switch_ft sft =
{
    .upm_switch_get_value = &upm_relay_get_value
};

const void* upm_relay_get_ft(upm_sensor_t sensor_type) {
    if(sensor_type == UPM_SENSOR) {
        return &ft;
    }
    if(sensor_type == UPM_SWITCH) {
        return &sft;
    }
    return NULL;
}

void* upm_relay_init_name(){
    return NULL;
}

void upm_relay_close(void* dev)
{
    relay_close((relay_context)dev);
}

upm_result_t upm_relay_get_value(void* dev, bool* value, int num)
{
    // num is unused

    if (relay_is_on((relay_context)dev))
        *value = true;
    else
        *value = false;

    return UPM_SUCCESS;
}
