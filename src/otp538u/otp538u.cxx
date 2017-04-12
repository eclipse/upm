/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015-2016 Intel Corporation.
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

#include "otp538u.hpp"

using namespace upm;
using namespace std;

OTP538U::OTP538U(int pinA, int pinO, float aref) :
    m_otp538u(otp538u_init(pinA, pinO, aref))
{
    if (!m_otp538u)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": otp538u_init() failed");
}

OTP538U::~OTP538U()
{
    otp538u_close(m_otp538u);
}

float OTP538U::ambientTemperature()
{
    float temp = 0;

    if (otp538u_get_ambient_temperature(m_otp538u, &temp))
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": otp538u_get_ambient_temperature() failed");

    return temp;
}

float OTP538U::objectTemperature()
{
    float temp = 0;

    if (otp538u_get_object_temperature(m_otp538u, &temp))
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": otp538u_get_object_temperature() failed");

    return temp;
}
