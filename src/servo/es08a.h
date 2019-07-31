/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 *         Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef ES08A_H_
#define ES08A_H_

#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "upm.h"
#include "mraa/pwm.h"

#define ES08A_MIN_PULSE_WIDTH             600
#define ES08A_MAX_PULSE_WIDTH             2200
#define ES08A_PERIOD                      20000
#define ES08A_MAX_ANGLE                   180.0

/**
 * @file es08a.h
 * @library servo
 * @brief C API for the ES08A Servo
 *
 * @include es08a.c
 */

/**
 * device context
 */
typedef struct _es08a_context {
    mraa_pwm_context    pwm;
    uint16_t            servo_pin;
    uint32_t            max_pulse_width;
    uint32_t            min_pulse_width;
} *es08a_context;

/**
 * Instantiates a the servo at the given pin
 *
 * @param pin Servo pin number
 * @param minPulseWidth Minimum pulse width, in microseconds
 * @param maxPulseWidth Maximum pulse width, in microseconds
 * @param waitAndDisablePwm If 1, PWM is enabled only during the
 * setAngle() execution for a period of 1 second, and then turned back
 * off. If 0, PWM remains on afterward.
 */


es08a_context es08a_init(int32_t pin, int32_t min_pulse_width,
                         int32_t max_pulse_width);

/**
 * Halts PWM for this servo and allows it to move freely.
 */
void es08a_halt(es08a_context dev);

/**
 * Sets the angle of the servo engine.
 *
 * @param angle Number between 0 and 180
 * @return 0 if successful, non-zero otherwise
 */
upm_result_t es08a_set_angle(es08a_context dev, int32_t angle);

/*
 * Calculating relative pulse time to the value.
 * */
upm_result_t es08a_calc_pulse_travelling(const es08a_context dev,
                                         int32_t* ret_val,
                                         int32_t value);

/**
 * Sets the minimum pulse width
 *
 * @param width Minimum HIGH signal width
 */
void es08a_set_min_pulse_width (es08a_context dev, int width);

/**
 * Sets the maximum pulse width
 *
 * @param width Maximum HIGH signal width
 */
void es08a_set_max_pulse_width (es08a_context dev, int width);

/**
 * Returns the minimum pulse width
 *
 * @return Minimum pulse width
 */
int es08a_get_min_pulse_width (es08a_context dev);

/**
 * Returns the maximum pulse width
 *
 * @return Maximum pulse width
 */
int es08a_get_max_pulse_width (es08a_context dev);

#endif /* ES08A_H_ */
