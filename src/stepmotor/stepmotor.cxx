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
#include <string>
#include <stdexcept>
#include <unistd.h>
#include <stdlib.h>

#include "stepmotor.h"

using namespace upm;

StepMotor::StepMotor (int dirPin, int stePin)
                    : m_pwmStepContext(stePin), m_dirPinCtx(dirPin) {
    mraa::Result error = mraa::SUCCESS;
    m_name = "StepMotor";

    m_stePin = stePin;
    m_dirPin = dirPin;

    error = m_dirPinCtx.dir (mraa::DIR_OUT);
    if (error != mraa::SUCCESS) {
        mraa::printError (error);
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

mraa::Result
StepMotor::stepForward (int ticks) {
    dirForward ();
    return move (ticks);
}

mraa::Result
StepMotor::stepBackwards (int ticks) {
    dirBackwards ();
    return move (ticks);
}

mraa::Result
StepMotor::move (int ticks) {
    mraa::Result error = mraa::SUCCESS;

    m_pwmStepContext.enable (1);
    for (int tick = 0; tick < ticks; tick++) {
        m_pwmStepContext.period_us (m_speed);
        m_pwmStepContext.pulsewidth_us (PULSEWIDTH);
    }
    m_pwmStepContext.enable (0);

    return error;
}

mraa::Result
StepMotor::dirForward () {
    mraa::Result error = mraa::SUCCESS;

    error = m_dirPinCtx.write (HIGH);
    if (error != mraa::SUCCESS) {
        mraa::printError (error);
    }

    return error;
}

mraa::Result
StepMotor::dirBackwards () {
    mraa::Result error = mraa::SUCCESS;

    error = m_dirPinCtx.write (LOW);
    if (error != mraa::SUCCESS) {
        mraa::printError (error);
    }

    return error;
}
