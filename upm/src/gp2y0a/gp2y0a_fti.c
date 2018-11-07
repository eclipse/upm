/*
 * Author: Jon Trulson <jtrulson@ics.com>
 *         Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
