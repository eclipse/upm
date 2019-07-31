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

#include <assert.h>
#include <upm_utilities.h>

#include "cjq4435.h"


cjq4435_context cjq4435_init(int pin)
{
    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        return NULL;
    }

    cjq4435_context dev =
        (cjq4435_context)malloc(sizeof(struct _cjq4435_context));

    if (!dev)
        return NULL;

    // zero out context
    memset((void *)dev, 0, sizeof(struct _cjq4435_context));

    if ( !(dev->pwm = mraa_pwm_init(pin)) )
    {
        printf("%s: mraa_pwm_init() failed.\n", __FUNCTION__);

        cjq4435_close(dev);
        return NULL;
    }

    dev->enabled = false;

    return dev;
}

void cjq4435_close(const cjq4435_context dev)
{
    assert(dev != NULL);

    if (dev->pwm)
    {
        if (dev->enabled)
            mraa_pwm_enable(dev->pwm, 0);

        mraa_pwm_close(dev->pwm);
    }

    free(dev);
}

upm_result_t cjq4435_set_period_us(const cjq4435_context dev, int us)
{
    assert(dev != NULL);

    if (mraa_pwm_period_us(dev->pwm, us))
    {
        printf("%s: period specified is not supported\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

upm_result_t cjq4435_set_period_ms(const cjq4435_context dev, int ms)
{
    assert(dev != NULL);

    if (mraa_pwm_period_ms(dev->pwm, ms))
    {
        printf("%s: period specified is not supported\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

upm_result_t cjq4435_set_period_seconds(const cjq4435_context dev,
                                        float seconds)
{
    assert(dev != NULL);

    if (mraa_pwm_period(dev->pwm, seconds))
    {
        printf("%s: period specified is not supported\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

upm_result_t cjq4435_enable(const cjq4435_context dev, bool enable)
{
    assert(dev != NULL);

    dev->enabled = enable;
    if (mraa_pwm_enable(dev->pwm, ((enable) ? 1 : 0)))
    {
        printf("%s: mraa_pwm_enable() failed\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

upm_result_t cjq4435_set_duty_cycle(const cjq4435_context dev,
                                    float dutyCycle)
{
    assert(dev != NULL);

    if (dutyCycle < 0.0)
        dutyCycle = 0.0;

    if (dutyCycle > 1.0)
        dutyCycle = 1.0;

    if (mraa_pwm_write(dev->pwm, dutyCycle))
    {
        printf("%s: mraa_pwm_write() failed\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

void cjq4435_on(const cjq4435_context dev)
{
    assert(dev != NULL);

    // set a 1 millisecond period, with 100% duty cycle

    cjq4435_enable(dev, false);
    cjq4435_set_period_us(dev, 1000);
    cjq4435_set_duty_cycle(dev, 1.0);
    cjq4435_enable(dev, true);
}

void cjq4435_off(const cjq4435_context dev)
{
    assert(dev != NULL);

    // set a 1 millisecond period, with 100% duty cycle

    cjq4435_enable(dev, false);
    cjq4435_set_period_us(dev, 1000);
    cjq4435_set_duty_cycle(dev, 0.0);
    cjq4435_enable(dev, true);
}
