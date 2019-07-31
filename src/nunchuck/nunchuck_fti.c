/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include "nunchuck.h"

#include "upm_fti.h"

/**
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_nunchuck_name[] = "NUNCHUCK";
const char upm_nunchuck_description[] = "Nunchuck controller";
const upm_protocol_t upm_nunchuck_protocol[] = {UPM_I2C};
const upm_sensor_t upm_nunchuck_category[] =
{ UPM_ACCELEROMETER, UPM_JOYSTICK, UPM_BUTTONS};

// forward declarations
const void* upm_nunchuck_get_ft(upm_sensor_t sensor_type);
void* upm_nunchuck_init_name();
void upm_nunchuck_close(void *dev);
upm_result_t upm_nunchuck_get_accel_values(void *dev, float *value, upm_acceleration_u unit);
upm_result_t upm_nunchuck_get_joystick_value_x(const void *dev, float *value);
upm_result_t upm_nunchuck_get_joystick_value_y(const void *dev, float *value);
upm_result_t upm_nunchuck_get_num_buttons(const void *dev,
                                          unsigned int *num_buttons);
upm_result_t upm_nunchuck_get_button_values(const void *dev,
                                            bool *values);

const upm_sensor_descriptor_t upm_nunchuck_get_descriptor()
{
    upm_sensor_descriptor_t usd;
    usd.name = upm_nunchuck_name;
    usd.description = upm_nunchuck_description;
    usd.protocol_size = 1;
    usd.protocol = upm_nunchuck_protocol;
    usd.category_size = 3;
    usd.category = upm_nunchuck_category;
    return usd;
}

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = upm_nunchuck_init_name,
    .upm_sensor_close = upm_nunchuck_close,
};

static const upm_acceleration_ft aft =
{
    .upm_acceleration_get_value = upm_nunchuck_get_accel_values
};

static const upm_joystick_ft jft =
{
    .upm_joystick_get_value_x = upm_nunchuck_get_joystick_value_x,
    .upm_joystick_get_value_y = upm_nunchuck_get_joystick_value_y
};

static const upm_buttons_ft bft =
{
    .upm_buttons_get_num_buttons = upm_nunchuck_get_num_buttons,
    .upm_buttons_get_values = upm_nunchuck_get_button_values
};

const void* upm_nunchuck_get_ft(upm_sensor_t sensor_type)
{
  switch(sensor_type)
    {
    case UPM_SENSOR:
      return &ft;

    case UPM_ACCELEROMETER:
      return &aft;

    case UPM_JOYSTICK:
      return &jft;

    case UPM_BUTTONS:
      return &bft;

    default:
      return NULL;
    }
}

void* upm_nunchuck_init_name()
{
    return NULL;
}

void upm_nunchuck_close(void *dev)
{
    nunchuck_close((nunchuck_context)dev);
}

upm_result_t upm_nunchuck_get_accel_values(void *dev,
                                           float *value,
                                           upm_acceleration_u unit)
{
    if (nunchuck_update((nunchuck_context)dev))
        return UPM_ERROR_OPERATION_FAILED;

    int x, y, z;
    nunchuck_get_acceleration((nunchuck_context)dev, &x, &y, &z);
    value[0] = (float)x;
    value[1] = (float)y;
    value[2] = (float)z;

    return UPM_SUCCESS;
}

upm_result_t upm_nunchuck_get_joystick_value_x(const void *dev, float *value)
{
    // This is bad - separating getting X and Y stick values... You
    // can't be sure when updating for one axis whether the other
    // changed... Should be fixed by having a single function return
    // both values from the same sampletime.  Same goes for the rest
    // of the values reported.
    if (nunchuck_update((nunchuck_context)dev))
        return UPM_ERROR_OPERATION_FAILED;

    int x;
    nunchuck_get_stick((nunchuck_context)dev, &x, NULL);
    *value = (float)x;

    return UPM_SUCCESS;
}

upm_result_t upm_nunchuck_get_joystick_value_y(const void *dev, float *value)
{
    // This is bad - separating getting X and Y stick values... You
    // can't be sure when updating for one axis whether the other
    // changed... Should be fixed by having a single function return
    // both values from the same sampletime.  Same goes for the rest
    // of the values reported.
    if (nunchuck_update((nunchuck_context)dev))
        return UPM_ERROR_OPERATION_FAILED;

    int y;
    nunchuck_get_stick((nunchuck_context)dev, NULL, &y);
    *value = (float)y;

    return UPM_SUCCESS;
}

upm_result_t upm_nunchuck_get_num_buttons(const void *dev,
                                          unsigned int *num_buttons)
{
    // always 2 buttons (C and Z)

    *num_buttons = 2;

    return UPM_SUCCESS;
}

upm_result_t upm_nunchuck_get_button_values(const void *dev,
                                            bool *values)
{
    if (nunchuck_update((nunchuck_context)dev))
        return UPM_ERROR_OPERATION_FAILED;

    bool bc, bz;
    nunchuck_get_buttons((nunchuck_context)dev, &bc, &bz);
    // hope they passed a bool[2]....
    values[0] = bc;
    values[1] = bz;

    return UPM_SUCCESS;
}
