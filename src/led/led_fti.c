/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * Contributions: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 * Contributions: Sarah Knepper <sarah.knepper@intel.com>
 *                Abhishek Malik <abhishek.malik@intel.com>
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
