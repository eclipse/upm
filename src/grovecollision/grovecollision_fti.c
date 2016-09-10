/*
 * Author: Zion Orent <sorent@ics.com>
 *         Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2015 Intel Corporation.
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
#include "grovecollision.h"
#include "upm_fti.h"

/** 
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_mvs0608_name[] = "MVS0608";
const char upm_mvs0608_description[] = "MVS0608 Grove Collision sensor";
const upm_protocol_t upm_mvs0608_protocol[] = {UPM_GPIO};
const upm_sensor_t upm_mvs0608_category[] = {UPM_ELECTRICITY};

// forward declarations
const upm_sensor_descriptor_t upm_mvs0608_get_descriptor();
const void* upm_mvs0608_get_ft(upm_sensor_t sensor_type);
void* upm_mvs0608_init_name();
void upm_mvs0608_close(void* dev);

const upm_sensor_descriptor_t upm_mvs0608_get_descriptor(){
    upm_sensor_descriptor_t usd;
    usd.name = upm_mvs0608_name;
    usd.description = upm_mvs0608_description;
    usd.protocol_size = 1;
    usd.protocol = upm_mvs0608_protocol;
    usd.category_size = 1;
    usd.category = upm_mvs0608_category;
    return usd;
}

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_mvs0608_init_name,
    .upm_sensor_close = &upm_mvs0608_close,
    .upm_sensor_get_descriptor = &upm_mvs0608_get_descriptor
};

const void* upm_mvs0608_get_ft(upm_sensor_t sensor_type){
    if(sensor_type == UPM_SENSOR){
        return &ft;
    }
    return NULL;
}

void* upm_mvs0608_init_name(){
    return NULL;
}

void upm_mvs0608_close(void* dev){
    mvs0608_close((mvs0608_context)dev);
}

upm_result_t upm_mvs0608_is_colliding(void* dev, bool* collision_val){
    return mvs0608_is_colliding((mvs0608_context)dev, collision_val);
}