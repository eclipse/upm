/*
 * Author: Sisinty Sasmita Patra <sisinty.s.patra@intel.com>
 *
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
