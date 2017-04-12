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
#include "a110x.h"
#include "upm_fti.h"

/**
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_a110x_name[] = "A110X";
const char upm_a110x_description[] = "A110X Hall Effect Sensor";
const upm_protocol_t upm_a110x_protocol[] = {UPM_GPIO};
const upm_sensor_t upm_a110x_category[] = {UPM_ELECTRICITY};

// forward declarations
const upm_sensor_descriptor_t upm_a110x_get_descriptor();
const void* upm_a110x_get_ft(upm_sensor_t sensor_type);
void* upm_a110x_init_name();
void upm_a110x_close(void* dev);

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_a110x_init_name,
    .upm_sensor_close = &upm_a110x_close,
    .upm_sensor_get_descriptor = &upm_a110x_get_descriptor
};

const void* upm_a110x_get_ft(upm_sensor_t sensor_type){
    if (sensor_type == UPM_SENSOR){
        return &ft;
    }
    return NULL;
}

const upm_sensor_descriptor_t upm_a110x_get_descriptor(){
    upm_sensor_descriptor_t usd;
    usd.name = upm_a110x_name;
    usd.description = upm_a110x_description;
    usd.protocol_size = 1;
    usd.protocol = upm_a110x_protocol;
    usd.category_size = 1;
    usd.category = upm_a110x_category;
    return usd;
}

void* upm_a110x_init_name(){
    return NULL;
}

void upm_a110x_close(void* dev){
    a110x_close((a110x_context)dev);
}
