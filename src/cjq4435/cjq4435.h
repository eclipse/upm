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
