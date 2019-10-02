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
#include "upm_string_parser.hpp"

using namespace upm;
using namespace std;

CJQ4435::CJQ4435(int pin) :
    m_cjq4435(cjq4435_init(pin))
{
    if (!m_cjq4435)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": cjq4435_init() failed");
}

CJQ4435::CJQ4435(std::string initStr) : mraaIo(initStr)
{
    mraa_io_descriptor* descs = mraaIo.getMraaDescriptors();
    std::vector<std::string> upmTokens;

    if(!mraaIo.getLeftoverStr().empty()) {
        upmTokens = UpmStringParser::parse(mraaIo.getLeftoverStr());
    }

    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": cjq4435_init() failed");
    }

    m_cjq4435 = (cjq4435_context)malloc(sizeof(struct _cjq4435_context));
    if(!m_cjq4435) {
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": cjq4435_init() failed");
    }

    // zero out context
    memset((void *)m_cjq4435, 0, sizeof(struct _cjq4435_context));

    if(!descs->pwms) {
        printf("%s: mraa_pwm_init() failed.\n", __FUNCTION__);
        cjq4435_close(m_cjq4435);
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": cjq4435_init() failed");
    } else {
        if( !(m_cjq4435->pwm = descs->pwms[0]) ) {
            printf("%s: mraa_pwm_init() failed.\n", __FUNCTION__);
            cjq4435_close(m_cjq4435);
            throw std::runtime_error(std::string(__FUNCTION__) +
                                     ": cjq4435_init() failed");
        }
    }

    m_cjq4435->enabled = false;

    for(std::string tok : upmTokens) {
        if(tok.substr(0, 12) == "setPeriodUS:") {
            int us = std::stoi(tok.substr(0, 12), nullptr, 0);
            setPeriodUS(us);
        }
        if(tok.substr(0, 12) == "setPeriodMS:") {
            int ms = std::stoi(tok.substr(0, 12), nullptr, 0);
            setPeriodMS(ms);
        }
        if(tok.substr(0, 17) == "setPeriodSeconds:") {
            float seconds = std::stof(tok.substr(0, 17));
            setPeriodSeconds(seconds);
        }
        if(tok.substr(0, 7) == "enable:") {
            bool en = std::stoi(tok.substr(0, 7), nullptr, 0);
            enable(en);
        }
        if(tok.substr(0, 13) == "setDutyCycle:") {
            float dutyCycle = std::stof(tok.substr(0, 13));
            setDutyCycle(dutyCycle);
        }
    }
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

