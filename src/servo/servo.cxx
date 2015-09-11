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
#include <sstream>
#include <string>
#include <stdexcept>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

#include "servo.h"

using namespace upm;

Servo::Servo (int pin) {
    init(pin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH, DEFAULT_WAIT_DISABLE_PWM);
}

Servo::Servo (int pin, int minPulseWidth, int maxPulseWidth) {
    init(pin, minPulseWidth, maxPulseWidth, DEFAULT_WAIT_DISABLE_PWM);
}

Servo::Servo (int pin, int minPulseWidth, int maxPulseWidth, int waitAndDisablePwm) {
    init(pin, minPulseWidth, maxPulseWidth, waitAndDisablePwm);
}

Servo::~Servo () {
    haltPwm();
    mraa_pwm_close (m_pwmServoContext);
}

/*
 * X = between (MIN_PULSE_WIDTH , MAX_PULSE_WIDTH)
 *
 * X usec
 * _______
 *        |_______________________________________
 *                      m_period usec
 *
 * */
mraa_result_t Servo::setAngle (int angle) {
    if (angle > m_maxAngle || angle < 0) {
        // C++11 std::to_string() would be nice, but...
        std::ostringstream str;
        str << m_maxAngle;
        throw std::out_of_range(std::string(__FUNCTION__) +
                                ": angle must be between 0 and " +
                                str.str());

        return MRAA_ERROR_UNSPECIFIED;
    }

    mraa_pwm_enable (m_pwmServoContext, 1);
    mraa_pwm_period_us (m_pwmServoContext, m_period);
    mraa_pwm_pulsewidth_us (m_pwmServoContext, calcPulseTraveling(angle));

    if (m_waitAndDisablePwm) {
        sleep(1); // we must make sure that we don't turn off PWM before the servo is done moving.
        haltPwm();
    }

    m_currAngle = angle;
    return MRAA_SUCCESS;
}

mraa_result_t Servo::haltPwm () {
    return mraa_pwm_enable (m_pwmServoContext, 0);
}

/*
 * Calculating relative pulse time to the value.
 * */
int Servo::calcPulseTraveling (int value) {
    // if bigger than the boundaries
    if (value > m_maxAngle) {
        return m_maxPulseWidth;
    }

    // if less than the boundaries
    if (value  < 0) {
        return m_minPulseWidth;
    }

    // the conversion
    return (int) ((float)m_minPulseWidth + ((float)value / m_maxAngle) * ((float)m_maxPulseWidth - (float)m_minPulseWidth));
}

void
Servo::setMinPulseWidth (int width) {
    m_minPulseWidth = width;
}

void
Servo::setMaxPulseWidth (int width) {
    m_maxPulseWidth = width;
}

void
Servo::setPeriod (int period) {
    m_period = period;
}

int
Servo::getMinPulseWidth () {
    return m_minPulseWidth;
}

int
Servo::getMaxPulseWidth () {
    return m_maxPulseWidth;
}

int
Servo::getPeriod () {
    return m_period;
}

/**
 *  private mathod:  would like to use delegating constructors instead but that requires C++11
 */
void
Servo::init (int pin, int minPulseWidth, int maxPulseWidth, int waitAndDisablePwm) {
    m_minPulseWidth   = minPulseWidth;
    m_maxPulseWidth   = maxPulseWidth;
    m_period          = PERIOD;

    m_waitAndDisablePwm = waitAndDisablePwm;

    m_maxAngle        = 180.0;
    m_servoPin        = pin;
    
    if ( !(m_pwmServoContext = mraa_pwm_init (m_servoPin)) ) 
      {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_pwm_init() failed, invalid pin?");
        return;
      }

    m_currAngle = 180;

    setAngle (0);
}
