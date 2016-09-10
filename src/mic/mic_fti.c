/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * 	   Sisinty Sasmita Patra <sisinty.s.patra@intel.com>
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

#include "mic.h"
#include "upm_fti.h"

/** 
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_mic_name[] = "Microphone";
const char upm_mic_description[] = "Analog Microphone";
const upm_protocol_t upm_mic_protocol[] = {UPM_ANALOG};
const upm_sensor_t upm_mic_category[] = {UPM_AUDIO};

// forward declarations
const upm_sensor_descriptor_t upm_mic_get_descriptor();
const void* upm_mic_get_ft(upm_sensor_t sensot_type);
void* upm_mic_init_name();
void upm_mic_close(void* dev);
upm_result_t upm_mic_get_value(void* dev, float* micval, upm_audio_u unit);

const upm_sensor_descriptor_t upm_mic_get_descriptor() {
    upm_sensor_descriptor_t usd;
    usd.name = upm_mic_name;
    usd.description = upm_mic_description;
    usd.protocol_size = 1;
    usd.protocol = upm_mic_protocol;
    usd.category_size = 1;
    usd.category = upm_mic_category;
    return usd;
}

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_mic_init_name,
    .upm_sensor_close = &upm_mic_close,
    .upm_sensor_get_descriptor = &upm_mic_get_descriptor
};

static const upm_audio_ft aft =
{
    .upm_audio_get_value = &upm_mic_get_value
};

const void* upm_mic_get_ft(upm_sensor_t sensor_type) {
    if(sensor_type == UPM_SENSOR) {
        return &ft;
    }
    if(sensor_type == UPM_AUDIO) {
        return &aft;
    }
    return NULL;
}

void* upm_mic_init_name(){
    return NULL;
}

void upm_mic_close(void* dev)
{
    mic_close((mic_context)dev);
}

upm_result_t upm_mic_get_value(void* dev, float *micval, upm_audio_u unit)
{
    return mic_get_value((mic_context)dev, micval);
}
