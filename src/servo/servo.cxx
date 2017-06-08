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

#include "servo.hpp"

using namespace upm;

Servo::Servo (std::string mraa_init_str):iMraa(mraa_init_str) {
    DEBUG_MSG("XXX");
}

Servo::Servo (int pin, int min_pw_us, int max_pw_us,
        int period_us, float max_angle): Servo("p:" + std::to_string(pin)) {
    DEBUG_MSG("XXX");

    m_min_pw_us = min_pw_us;
    m_max_pw_us = max_pw_us;
    m_period_us = period_us;
    m_max_angle = max_angle;
}


Servo::~Servo () {
    DEBUG_MSG("XXX");

    /* On a clean exit, disable the target PWM */
    if (!_pwm.empty())
        _pwm.front()->enable(false);
}

/*
 * X = between (MIN_PULSE_WIDTH , MAX_PULSE_WIDTH)
 *
 * X usec
 * _______
 *        |_______________________________________
 *                      m_period_us usec
 *
 * */
mraa_result_t Servo::setAngle (int angle) {
    if (angle > m_max_angle || angle < 0) {
        // C++11 std::to_string() would be nice, but...
        std::ostringstream str;
        str << m_max_angle;
        throw std::out_of_range(std::string(__FUNCTION__) +
                                ": angle must be between 0 and " +
                                str.str());

        return MRAA_ERROR_UNSPECIFIED;
    }

    _pwm.front()->enable(true);
    _pwm.front()->period_us(m_period_us);
    _pwm.front()->pulsewidth_us(calcPulseTraveling(angle));

    return MRAA_SUCCESS;
}

void Servo::AngleForCommands(std::map<std::string, float> commands)
{
    for(std::map<std::string, float>::const_iterator it = commands.begin();
            it != commands.end(); ++it)
    {
        if (std::find(Commands().begin(), Commands().end(), it->first) != Commands().end())
            setAngle(it->second);
    }
}

/*
 * Calculating relative pulse time to the value.
 * */
int Servo::calcPulseTraveling (int value) {
    // if bigger than the boundaries
    if (value > m_max_angle) {
        return m_max_pw_us;
    }

    // if less than the boundaries
    if (value  < 0) {
        return m_min_pw_us;
    }

    // the conversion
    return (int) ((float)m_min_pw_us + ((float)value / m_max_angle) * ((float)m_max_pw_us - (float)m_min_pw_us));
}

void
Servo::setMinPulseWidth (int width) {
    m_min_pw_us = width;
}

void
Servo::setMaxPulseWidth (int width) {
    m_max_pw_us = width;
}

void
Servo::setPeriod (int period) {
    m_period_us = period;
}

int
Servo::getMinPulseWidth () {
    return m_min_pw_us;
}

int
Servo::getMaxPulseWidth () {
    return m_max_pw_us;
}

int
Servo::getPeriod () {
    return m_period_us;
}
