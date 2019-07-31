/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015-2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
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

float OTP538U::getTemperature()
{
    return ambientTemperature();
}

float OTP538U::objectTemperature()
{
    float temp = 0;

    if (otp538u_get_object_temperature(m_otp538u, &temp))
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": otp538u_get_object_temperature() failed");

    return temp;
}
