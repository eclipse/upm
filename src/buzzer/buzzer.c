/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * based on original C++ driver by
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <assert.h>
#include <unistd.h>
#include <upm_utilities.h>
#include "buzzer.h"

buzzer_context buzzer_init(int pin)
{
    buzzer_context dev =
        (buzzer_context)malloc(sizeof(struct _buzzer_context));

    if (!dev)
        return NULL;

    dev->pwm = NULL;
    dev->volume = 0.0;
    dev->initialized = false;

    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        buzzer_close(dev);
        return NULL;
    }

    // MRAA contexts...
    if ( !(dev->pwm = mraa_pwm_init(pin)) )
    {
        printf("%s: mraa_pwm_init() failed\n",
               __FUNCTION__);
        buzzer_close(dev);
        return NULL;
    }

    mraa_pwm_enable(dev->pwm, 1);

    buzzer_set_volume(dev, 0.01);

    dev->initialized = true;
    return dev;
}

void buzzer_close(buzzer_context dev)
{
    assert(dev != NULL);

    if (dev->initialized)
        buzzer_stop_sound(dev);

    if (dev->pwm)
    {
        mraa_pwm_enable(dev->pwm, 0);
        mraa_pwm_close(dev->pwm);
    }

    free(dev);
}

void buzzer_set_volume(const buzzer_context dev, float vol)
{
    assert(dev != NULL);

    dev->volume = vol;
}

float buzzer_get_volume(const buzzer_context dev)
{
    assert(dev != NULL);

    return dev->volume;
}

upm_result_t buzzer_play_sound(const buzzer_context dev, int note, int delay)
{
    assert(dev != NULL);

    if (mraa_pwm_period_us(dev->pwm, note) != MRAA_SUCCESS)
    {
        printf("%s: mraa_pwm_period_us() did not succeed\n", __FUNCTION__);
    }

    if (mraa_pwm_write(dev->pwm, dev->volume * 0.5))
    {
        printf("%s: mraa_pwm_write() failed\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    if (delay >= 0)
    {
        upm_delay_us(delay);
        return buzzer_stop_sound(dev);
    }

    return UPM_SUCCESS;
}

upm_result_t buzzer_stop_sound(const buzzer_context dev)
{
    assert(dev != NULL);

    mraa_result_t rv = MRAA_SUCCESS;

    rv = mraa_pwm_period_us(dev->pwm, 1);
    rv = mraa_pwm_write(dev->pwm, 0);

    if (rv)
    {
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}
