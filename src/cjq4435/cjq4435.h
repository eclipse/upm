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
#pragma once

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <mraa/pwm.h>
#include <upm.h>

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @file cjq4435.h
     * @library cjq4435
     * @brief API CJQ4435 MOSFET
     *
     * @include cjq4435.c
     */

    /**
     * Device context
     */
    typedef struct _cjq4435_context {
        mraa_pwm_context pwm;

        bool enabled;
    } *cjq4435_context;

    /**
     * CJQ4435 initializer
     *
     * @param pin Digital pin to use; this pin must be PWM-capable
     * @return Initialized device context, or NULL on error.
     */
    cjq4435_context cjq4435_init(int pin);

    /**
     * CJQ4435 destructor
     *
     * @param dev Device context
     */
    void cjq4435_close(cjq4435_context dev);

    /**
     * Sets a period in microseconds
     *
     * @param dev Device context
     * @param us Period in microseconds
     * @return UPM result
     */
    upm_result_t cjq4435_set_period_us(const cjq4435_context dev, int us);

    /**
     * Sets a period in milliseconds
     *
     * @param dev Device context
     * @param ms Period in milliseconds
     * @return UPM result
     */
    upm_result_t cjq4435_set_period_ms(const cjq4435_context dev, int us);

    /**
     * Sets a period in seconds
     *
     * @param dev Device context
     * @param seconds Period in seconds
     * @return UPM result
     */
    upm_result_t cjq4435_set_period_seconds(const cjq4435_context dev,
                                            float seconds);

    /**
     * Enables output
     *
     * @param dev Device context
     * @param enable Enables PWM output if true, disables otherwise
     * @return UPM result
     */
    upm_result_t cjq4435_enable(const cjq4435_context dev, bool enable);

    /**
     * Sets a duty cycle. Duty cycle is a floating-point number
     * between 0.0 (always off) and 1.0 (always on). It represents a
     * proportion of time, per period, during which the output is
     * driven high.
     *
     * @param dev Device context
     * @param dutyCycle Duty cycle to use
     * @return UPM result
     */
    upm_result_t cjq4435_set_duty_cycle(const cjq4435_context dev,
                                        float dutyCycle);

    /**
     * Shortcut to turn the output to continuous on (high)
     *
     * @param dev Device context
     */
    void cjq4435_on(const cjq4435_context dev);

    /**
     * Shortcut to turn the output to continuous off (low)
     *
     * @param dev Device context
     */
    void cjq4435_off(const cjq4435_context dev);


#ifdef __cplusplus
}
#endif
