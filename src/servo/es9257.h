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

#ifndef ES9257_H_
#define ES9257_H_

#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "upm.h"
#include "mraa/pwm.h"

#define ES9257_MIN_PULSE_WIDTH             475
#define ES9257_MAX_PULSE_WIDTH             2100
#define ES9257_PERIOD                      20000
#define ES9257_MAX_ANGLE                   180.0

/**
 * @file es9257.h
 * @library servo
 * @brief C API for the ES9257 Servo
 *
 * @include es9257.c
 */

/*
 * device context
 */
typedef struct _es9257_context {
    mraa_pwm_context    pwm;
    uint16_t            servo_pin;
    uint32_t            max_pulse_width;
    uint32_t            min_pulse_width;
} *es9257_context;

/**
 * Instantiates a the servo at the given pin
 *
 * @param pin Servo pin number
 * @param minPulseWidth Minimum pulse width, in microseconds
 * @param maxPulseWidth Maximum pulse width, in microseconds
 */


es9257_context es9257_init(int32_t pin, int32_t min_pulse_width,
                         int32_t max_pulse_width);

/**
 * Halts PWM for this servo and allows it to move freely.
 */
void es9257_halt(es9257_context dev);

/**
 * Sets the angle of the servo engine.
 *
 * @param angle Number between 0 and 180
 * @return 0 if successful, non-zero otherwise
 */
upm_result_t es9257_set_angle(es9257_context dev, int32_t angle);

/*
 * Calculating relative pulse time to the value.
 * */
upm_result_t es9257_calc_pulse_travelling(const es9257_context dev,
                                         int32_t* ret_val,
                                         int32_t value);

/**
 * Sets the minimum pulse width
 *
 * @param width Minimum HIGH signal width
 */
void es9257_set_min_pulse_width (es9257_context dev, int width);

/**
 * Sets the maximum pulse width
 *
 * @param width Maximum HIGH signal width
 */
void es9257_set_max_pulse_width (es9257_context dev, int width);

/**
 * Returns the minimum pulse width
 *
 * @return Minimum pulse width
 */
int es9257_get_min_pulse_width (es9257_context dev);

/**
 * Returns the maximum pulse width
 *
 * @return Maximum pulse width
 */
int es9257_get_max_pulse_width (es9257_context dev);

#endif /* ES9257_H_ */