/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * based on original C++ driver by
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
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

    buzzer_set_volume(dev, 1.0);

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

    if (mraa_pwm_period_us(dev->pwm, note))
    {
        printf("%s: mraa_pwm_period_us() failed\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
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
