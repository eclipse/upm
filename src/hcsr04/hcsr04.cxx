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
#include <functional>

#include "hcsr04.h"

using namespace upm;

#ifdef SWIGJAVA
HCSR04::HCSR04 (uint8_t triggerPin, uint8_t echoPin, IsrCallback *cb)
{
        HCSR04 (triggerPin, echoPin, generic_callback_isr);
}
#endif

HCSR04::HCSR04 (uint8_t triggerPin, uint8_t echoPin, void (*fptr)(void *)) {
    mraa_result_t error  = MRAA_SUCCESS;
    m_name              = "HCSR04";

    m_pwmTriggerCtx     = mraa_pwm_init (triggerPin);
    if (m_pwmTriggerCtx == NULL) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_pwm_init() failed, invalid pin?");
        return;
    }

    mraa_init();
    m_echoPinCtx = mraa_gpio_init(echoPin);
    if (m_echoPinCtx == NULL) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_gpio_init() failed, invalid pin?");
        return;
    }

    mraa_gpio_dir(m_echoPinCtx, MRAA_GPIO_IN);
    mraa_gpio_isr(m_echoPinCtx, MRAA_GPIO_EDGE_BOTH, fptr, NULL);
}

HCSR04::~HCSR04 () {
    mraa_result_t error = MRAA_SUCCESS;

    mraa_pwm_close (m_pwmTriggerCtx);
    error = mraa_gpio_close (m_echoPinCtx);
    if (error != MRAA_SUCCESS) {
        mraa_result_print (error);
    }
}

int
HCSR04::getDistance () {
    mraa_pwm_enable (m_pwmTriggerCtx, 1);
    mraa_pwm_period_us (m_pwmTriggerCtx, MAX_PERIOD);
    mraa_pwm_pulsewidth_us (m_pwmTriggerCtx, TRIGGER_PULSE);
    mraa_pwm_enable (m_pwmTriggerCtx, 0);

    m_doWork = 0;
    m_InterruptCounter = 0;
    while (!m_doWork) {
        sleep (1);
    }

    return m_FallingTimeStamp - m_RisingTimeStamp;
}

void
HCSR04::ackEdgeDetected () {
    struct timeval timer;
    gettimeofday(&timer, NULL);

    ++m_InterruptCounter;
    if (!(m_InterruptCounter % 2)) {
        m_FallingTimeStamp  = 1000000 * timer.tv_sec + timer.tv_usec;
        m_doWork = 1;
    } else {
        m_RisingTimeStamp = 1000000 * timer.tv_sec + timer.tv_usec;
    }
}
