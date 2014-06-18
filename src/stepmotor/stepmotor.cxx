/*
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

#include <iostream>
#include <unistd.h>
#include <stdlib.h>

#include "stepmotor.h"

using namespace upm;

StepMotor::StepMotor (int dirPin, int stePin) {
    maa_result_t error = MAA_SUCCESS;
    m_name = "StepMotor";

    maa_init();

    m_stePin = stePin;
    m_dirPin = dirPin;

    m_pwmStepContext = maa_pwm_init (m_stePin);
    m_dirPinCtx = maa_gpio_init (m_dirPin);
    if (m_dirPinCtx == NULL) {
        fprintf (stderr, "Are you sure that pin%d you requested is valid on your platform?", m_dirPin);
        exit (1);
    }

    error = maa_gpio_dir (m_dirPinCtx, MAA_GPIO_OUT);
    if (error != MAA_SUCCESS) {
        maa_result_print (error);
    }
}

StepMotor::~StepMotor() {
    maa_result_t error = MAA_SUCCESS;

    maa_pwm_close (m_pwmStepContext);

    error = maa_gpio_close (m_dirPinCtx);
    if (error != MAA_SUCCESS) {
        maa_result_print(error);
    }
}

void
StepMotor::setSpeed (int speed) {
    if (speed > MAX_PERIOD) {
        m_speed = MAX_PERIOD;
    }

    if (speed < MIN_PERIOD) {
        m_speed = MIN_PERIOD;
    }

    m_speed = speed;
}

maa_result_t
StepMotor::stepForward (int ticks) {
    dirForward ();
    move (ticks);
}

maa_result_t
StepMotor::stepBackwards (int ticks) {
    dirBackwards ();
    move (ticks);
}

maa_result_t
StepMotor::move (int ticks) {
    maa_result_t error = MAA_SUCCESS;

    maa_pwm_enable (m_pwmStepContext, 1);
    for (int tick = 0; tick < ticks; tick++) {
        maa_pwm_period_us (m_pwmStepContext, m_speed);
        maa_pwm_pulsewidth_us (m_pwmStepContext, PULSEWIDTH);
    }
    maa_pwm_enable (m_pwmStepContext, 0);

    return error;
}

maa_result_t
StepMotor::dirForward () {
    maa_result_t error = MAA_SUCCESS;

    error = maa_gpio_write (m_dirPinCtx, HIGH);
    if (error != MAA_SUCCESS) {
        maa_result_print (error);
    }

    return error;
}

maa_result_t
StepMotor::dirBackwards () {
    maa_result_t error = MAA_SUCCESS;

    error = maa_gpio_write (m_dirPinCtx, LOW);
    if (error != MAA_SUCCESS) {
        maa_result_print (error);
    }

    return error;
}
