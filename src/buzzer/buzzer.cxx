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

#include "buzzer.h"

using namespace upm;
using namespace std;

Buzzer::Buzzer(int pinNumber) {
    m_pwm_context = mraa_pwm_init(pinNumber);
    if(m_pwm_context == 0)
    {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_pwm_init() failed, invalid pin?");
        return;
    }
    m_name = "Buzzer";
    mraa_pwm_enable (m_pwm_context, 1);
    Buzzer::setVolume(1.0);
}

void Buzzer::setVolume(float vol){
    m_volume = vol;
}

float Buzzer::getVolume(){
    return m_volume;
}

int Buzzer::playSound(int note, int delay) {
    mraa_pwm_period_us(m_pwm_context, note);
    mraa_pwm_write(m_pwm_context, m_volume * 0.5);
    if(delay){
        usleep(delay);
        Buzzer::stopSound();
    }
    return note;
}

void Buzzer::stopSound(){
    // Has to be checked out on a scope and make sure it's flat 0
    mraa_pwm_period_us(m_pwm_context, 1);
    mraa_pwm_write(m_pwm_context, 0);
}

Buzzer::~Buzzer() {
    Buzzer::stopSound();
    mraa_pwm_enable(m_pwm_context, 0);
    mraa_pwm_close(m_pwm_context);
}

