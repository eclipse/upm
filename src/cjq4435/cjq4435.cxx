/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015-2017 Intel Corporation.
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

#include "cjq4435.hpp"

using namespace upm;
using namespace std;

CJQ4435::CJQ4435(int pin) :
    m_cjq4435(cjq4435_init(pin))
{
    if (!m_cjq4435)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": cjq4435_init() failed");
}

CJQ4435::~CJQ4435()
{
    cjq4435_close(m_cjq4435);
}

void CJQ4435::setPeriodUS(int us)
{
    if (cjq4435_set_period_us(m_cjq4435, us))
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": cjq4435_set_period_us() failed");
}

void CJQ4435::setPeriodMS(int ms)
{
    if (cjq4435_set_period_ms(m_cjq4435, ms))
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": cjq4435_set_period_ms() failed");
}

void CJQ4435::setPeriodSeconds(float seconds)
{
    if (cjq4435_set_period_seconds(m_cjq4435, seconds))
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": cjq4435_set_period_seconds() failed");
}

void CJQ4435::enable(bool enable)
{
    if (cjq4435_enable(m_cjq4435, enable))
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": cjq4435_enable() failed");
}

void CJQ4435::setDutyCycle(float dutyCycle)
{
    if (cjq4435_set_duty_cycle(m_cjq4435, dutyCycle))
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": cjq4435_set_duty_cycle() failed");
}

void CJQ4435::on()
{
    cjq4435_on(m_cjq4435);
}

void CJQ4435::off()
{
    cjq4435_off(m_cjq4435);
}

