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

#include "servo.h"

using namespace upm;

Servo::Servo (int pin) {
    maa_result_t error = MAA_SUCCESS;

    m_maxAngle        = 180.0;
    m_servoPin        = pin;
    m_pwmServoContext = maa_pwm_init (m_servoPin);
}

Servo::~Servo () {

}

/*
 * X = between (MIN_PULSE_WIDTH , MAX_PULSE_WIDTH)
 *
 * X usec
 * _______
 *        |_______________________________________
 *                      20000 usec
 *
 * Max period can be only 7968750(nses) which is ~8(msec)
 * so the servo wil not work as expected.
 * */
maa_result_t Servo::setAngle (int angle) {
    if (m_pwmServoContext == NULL) {
        std::cout << "PWM context is NULL" << std::endl;
        return MAA_ERROR_UNSPECIFIED;
    }

    maa_pwm_enable (m_pwmServoContext, 1);
    for (int cycles = 0; cycles < 128; cycles++) {
        maa_pwm_period_us (m_pwmServoContext, MAX_PERIOD);
        maa_pwm_pulsewidth_us (m_pwmServoContext, calcPulseTraveling(angle));
    }
    maa_pwm_enable (m_pwmServoContext, 0);

    std::cout << "angle = " << angle << " ,pulse = " << calcPulseTraveling(angle) << std::endl;
}

/*
 * Calculating relative pulse time to the value.
 * */
int Servo::calcPulseTraveling (int value) {
    // if bigger than the boundaries
    if (value > m_maxAngle) {
        return MAX_PULSE_WIDTH;
    }

    // if less than the boundaries
    if (value  < 0) {
        return MIN_PULSE_WIDTH;
    }

    // the conversion
    return (int) ((float)MIN_PULSE_WIDTH + ((float)value / m_maxAngle) * ((float)MAX_PULSE_WIDTH - (float)MIN_PULSE_WIDTH));
}
