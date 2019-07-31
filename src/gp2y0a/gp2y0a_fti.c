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
#include "gp2y0a.h"

#include "upm_fti.h"
#include "fti/upm_sensor.h"

/**
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_gp2y0a_name[] = "GP2Y0A";
const char upm_gp2y0a_description[] = "Grove 80cm IR Proximity Sensor";
const upm_protocol_t upm_gp2y0a_protocol[] = {UPM_ANALOG};
const upm_sensor_t upm_gp2y0a_category[] = {UPM_DISTANCE};

// forward declarations
const void* upm_gp2y0a_get_ft(upm_sensor_t sensor_type);
const upm_sensor_descriptor_t upm_gp2y0a_get_descriptor();
void* upm_gp2y0a_init_name();
void upm_gp2y0a_close(void* dev);

const upm_sensor_descriptor_t upm_gp2y0a_get_descriptor(){
    upm_sensor_descriptor_t usd;
    usd.name = upm_gp2y0a_name;
    usd.description = upm_gp2y0a_description;
    usd.protocol_size = 1;
    usd.protocol = upm_gp2y0a_protocol;
    usd.category_size = 1;
    usd.category = upm_gp2y0a_category;
    return usd;
}

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_gp2y0a_init_name,
    .upm_sensor_close = &upm_gp2y0a_close,
    .upm_sensor_get_descriptor = &upm_gp2y0a_get_descriptor
};

const void* upm_gp2y0a_get_ft(upm_sensor_t sensor_type)
{
    switch(sensor_type)
    {
        case UPM_SENSOR:
            return &ft;
        default:
            return NULL;
    }
}

void* upm_gp2y0a_init_name(){
    return NULL;
}

void upm_gp2y0a_close(void* dev){
    gp2y0a_close((gp2y0a_context)dev);
}
