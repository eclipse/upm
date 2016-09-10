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
#include "loudness.h"
#include "upm_fti.h"

/**
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_loudness_name[] = "loudness";
const char upm_loudness_description[] = " Loudness Sensor";
const upm_protocol_t upm_loudness_protocol[] = {UPM_ANALOG};
const upm_sensor_t upm_loudness_category[] = {UPM_AUDIO};

// forward declarations
const void* upm_loudness_get_ft(upm_sensor_t sensor_type);
const upm_sensor_descriptor_t upm_loudness_get_descriptor();
void* upm_loudness_init_name();
void upm_loudness_close(void* dev);


const upm_sensor_descriptor_t upm_loudness_get_descriptor(){
    upm_sensor_descriptor_t usd;
    usd.name = upm_loudness_name;
    usd.description = upm_loudness_description;
    usd.protocol_size = 1;
    usd.protocol = upm_loudness_protocol;
    usd.category_size = 1;
    usd.category = upm_loudness_category;
    return usd;
}

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_loudness_init_name,
    .upm_sensor_close = &upm_loudness_close,
    .upm_sensor_get_descriptor = &upm_loudness_get_descriptor
};

const void* upm_loudness_get_ft(upm_sensor_t sensor_type){
    if(sensor_type == UPM_SENSOR){
        return &ft;
    }
    return NULL;
}

void* upm_loudness_init_name(){
    return NULL;
}

void upm_loudness_close(void* dev){
    loudness_close((loudness_context)dev);
}

upm_result_t upm_loudness_get_value(void* dev, int* val){
    return loudness_get_value((loudness_context)dev, val);
}