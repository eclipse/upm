/*
 * Authors: Brendan Le Foll <brendan.le.foll@intel.com>
 *          Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 *          Sarah Knepper <sarah.knepper@intel.com>
 * Copyright (c) 2014 - 2016 Intel Corporation.
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

#include "temperature.hpp"
#include "math.h"

using namespace std;
using namespace upm;

Temperature::Temperature(unsigned int pin, float scale, int r0, int b)
{
    if ( !(m_aio = mraa_aio_init(pin)) ) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_aio_init() failed, invalid pin?");
        return;
    }
    m_scale = scale;
    m_r0 = r0;
    m_b = b;
}

Temperature::~Temperature()
{
    mraa_aio_close(m_aio);
}

int Temperature::value ()
{
    float a = (float) mraa_aio_read(m_aio);
    if (a == -1.0) return -1;
    // Apply scale factor after error check
    a *= m_scale;
    float r = (float)(1023.0-a)*(float)m_r0/a;
    float t = 1.0/(log(r/(float)m_r0)/(float)m_b + 1.0/298.15)-273.15;
    return (int) round(t);
}

float Temperature::getTemperature ()
{
    return value();
}

float Temperature::raw_value()
{
    return (float) mraa_aio_read(m_aio);
}
