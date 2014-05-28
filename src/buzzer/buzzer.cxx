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

#include "buzzer.h"

using namespace upm;

Buzzer::Buzzer (int pinNumber) {
	m_pwm_context = maa_pwm_init (pinNumber);
	m_name = "Buzzer";
}

int Buzzer::playSound (int note) {
	maa_pwm_enable (m_pwm_context, 1);
	maa_pwm_period_us (m_pwm_context, note);
	maa_pwm_write (m_pwm_context, 50.0);
	usleep (10000);
	maa_pwm_enable (m_pwm_context, 0);

	return 0;
}

Buzzer::~Buzzer() {
	maa_pwm_close(m_pwm_context);
	std::cout << "executed maa_pwm_close" << std::endl;
}

