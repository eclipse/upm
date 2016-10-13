/*
 * Author: Jon Trulson <jtrulson@ics.com>
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

#include "rotaryencoder.h"

#include <stdio.h>
#include <stdlib.h>

#include "upm_fti.h"
#include "upm_sensor.h"

/**
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_rotaryencoder_name[] = "ROTARYENCODER";
const char upm_rotaryencoder_description[] = "Rotary Encoder";
const upm_protocol_t upm_rotaryencoder_protocol[] = {UPM_GPIO, UPM_GPIO};
const upm_sensor_t upm_rotaryencoder_category[] = {UPM_ROTARYENCODER};

// forward declarations
const upm_sensor_descriptor_t upm_rotaryencoder_get_descriptor();
const void* upm_rotaryencoder_get_ft(upm_sensor_t sensor_type);
void* upm_rotaryencoder_init_name();
void upm_rotaryencoder_close(void* dev);
upm_result_t upm_rotaryencoder_get_pos(const void* dev, int *pos);
upm_result_t upm_rotaryencoder_set_pos(const void* dev, int pos);

const upm_sensor_descriptor_t upm_rotaryencoder_get_descriptor()
{
    upm_sensor_descriptor_t usd;
    usd.name = upm_rotaryencoder_name;
    usd.description = upm_rotaryencoder_description;
    usd.protocol_size = 2;
    usd.protocol = upm_rotaryencoder_protocol;
    usd.category_size = 1;
    usd.category = upm_rotaryencoder_category;
    return usd;
}

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = upm_rotaryencoder_init_name,
    .upm_sensor_close = upm_rotaryencoder_close,
    .upm_sensor_get_descriptor = upm_rotaryencoder_get_descriptor
};

static const upm_rotaryencoder_ft rft =
{
    .upm_rotaryencoder_get_position = upm_rotaryencoder_get_pos,
    .upm_rotaryencoder_set_position = upm_rotaryencoder_set_pos,
};

const void* upm_rotaryencoder_get_ft(upm_sensor_t sensor_type)
{
    if (sensor_type == UPM_SENSOR)
        return &ft;

    if (sensor_type == UPM_ROTARYENCODER)
        return &rft;

    return NULL;
}

void* upm_rotaryencoder_init_name()
{
    return NULL;
}

void upm_rotaryencoder_close(void* dev)
{
    rotaryencoder_close((rotaryencoder_context)dev);
}

upm_result_t upm_rotaryencoder_set_pos(const void *dev, int pos)
{
    rotaryencoder_set_position((rotaryencoder_context)dev, pos);
    return UPM_SUCCESS;
}

upm_result_t upm_rotaryencoder_get_pos(const void *dev, int *pos)
{
    *pos = rotaryencoder_get_position((rotaryencoder_context)dev);
    return UPM_SUCCESS;
}
