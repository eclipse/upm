/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
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

#include "ds18b20.h"

#include <stdio.h>
#include <stdlib.h>

#include "upm_fti.h"
#include "upm_sensor.h"

/**
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_ds18b20_name[] = "DS18B20";
const char upm_ds18b20_description[] = "DS18B20 DS 1-wire Temperature Sensor";
const upm_protocol_t upm_ds18b20_protocol[] = {UPM_UART};
const upm_sensor_t upm_ds18b20_category[] = {UPM_TEMPERATURE};

// forward declarations
const upm_sensor_descriptor_t upm_ds18b20_get_descriptor();
const void* upm_ds18b20_get_ft(upm_sensor_t sensor_type);
upm_result_t upm_ds18b20_get_value(void* dev, float* tempval,
                                upm_temperature_u unit);
void* upm_ds18b20_init_name();
void upm_ds18b20_close(void* dev);

const upm_sensor_descriptor_t upm_ds18b20_get_descriptor()
{
    upm_sensor_descriptor_t usd;
    usd.name = upm_ds18b20_name;
    usd.description = upm_ds18b20_description;
    usd.protocol_size = 1;
    usd.protocol = upm_ds18b20_protocol;
    usd.category_size = 1;
    usd.category = upm_ds18b20_category;
    return usd;
}

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_ds18b20_init_name,
    .upm_sensor_close = &upm_ds18b20_close,
    .upm_sensor_get_descriptor = &upm_ds18b20_get_descriptor
};

static const upm_temperature_ft tft =
{
    .upm_temperature_get_value = &upm_ds18b20_get_value,
};

const void* upm_ds18b20_get_ft(upm_sensor_t sensor_type)
{
    if (sensor_type == UPM_SENSOR)
        return &ft;

    if (sensor_type == UPM_TEMPERATURE)
        return &tft;

    return NULL;
}

void* upm_ds18b20_init_name()
{
    return NULL;
}

void upm_ds18b20_close(void* dev)
{
    ds18b20_close((ds18b20_context)dev);
}

// FTI/temp has no way to indicate devices on a bus, so we always use
// index 0 (the first detected device) for all queries.
upm_result_t upm_ds18b20_get_value(void* dev, float* tempval,
                                   upm_temperature_u unit)
{
    ds18b20_update((ds18b20_context)dev, 0);
    // always returned in C
    float temp = ds18b20_get_temperature((ds18b20_context)dev, 0);

    switch (unit)
        {
        case CELSIUS:
            *tempval = temp;
            return UPM_SUCCESS;

        case KELVIN:
            *tempval = temp + 273.15;
            return UPM_SUCCESS;

        case FAHRENHEIT:
            *tempval = temp * (9.0/5.0) + 32.0;
            return UPM_SUCCESS;
        }

    return UPM_SUCCESS;
}
