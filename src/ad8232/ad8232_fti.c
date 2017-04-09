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

#include "ad8232.h"
#include "upm_fti.h"

/** 
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_ad8232_name[] = "AD8232";
const char upm_ad8232_description[] = "AD8232 Heart Rate Monitor";
const upm_protocol_t upm_ad8232_protocol[] = {UPM_ANALOG, UPM_GPIO};
const upm_sensor_t upm_ad8232_category[] = {UPM_HEART_RATE};

// forward declarations
upm_sensor_descriptor_t upm_ad8232_get_descriptor (void* dev);
const void* upm_ad8232_get_ft(upm_sensor_t sensor_type);
void* upm_ad8232_init_name();
void upm_ad8232_close(void* dev);
upm_result_t upm_ad8232_get_value(void* dev, int* value,
                                  upm_heart_rate_u rate_unit);

upm_sensor_descriptor_t upm_ad8232_get_descriptor (void* dev){
    upm_sensor_descriptor_t usd;
    usd.name = upm_ad8232_name;
    usd.description = upm_ad8232_description;
    usd.protocol_size = 2;
    usd.protocol = upm_ad8232_protocol;
    usd.category_size = 1;
    usd.category = upm_ad8232_category;
    return usd;
}

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_ad8232_init_name,
    .upm_sensor_close = &upm_ad8232_close,
    //.upm_sensor_get_descriptor = &upm_ad8232_get_descriptor
};

static const upm_heart_rate_ft hft =
{
    .upm_heart_rate_get_value = &upm_ad8232_get_value
};

const void* upm_ad8232_get_ft(upm_sensor_t sensor_type){
    if(sensor_type == UPM_HEART_RATE){
        return &hft;
    }
    else if(sensor_type == UPM_SENSOR){
        return &ft;
    }

    return NULL;
}

void* upm_ad8232_init_name(){
    return NULL;
}

void upm_ad8232_close(void* dev){
    ad8232_close(dev);
}

upm_result_t upm_ad8232_get_value(void* dev, int* value,
                                  upm_heart_rate_u rate_unit){
    // rate_unit does not apply to this device, so...
    return ad8232_get_value(dev, value);
}