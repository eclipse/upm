/*
 * Authors: Brendan Le Foll <brendan.le.foll@intel.com>
 *          Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 *          Sarah Knepper <sarah.knepper@intel.com>
 * Copyright (c) 2014 - 2016 Intel Corporation.
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

#include "grovetemp.hpp"
#include "math.h"

using namespace upm;

GroveTemp::GroveTemp(unsigned int pin, float scale, int r0, int b)
{
    if ( !(m_aio = mraa_aio_init(pin)) ) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_aio_init() failed, invalid pin?");
        return;
    }
    m_name = "Temperature Sensor";
    m_scale = scale;
    m_r0 = r0;
    m_b = b;
}

GroveTemp::~GroveTemp()
{
    mraa_aio_close(m_aio);
}

int GroveTemp::value ()
{
    float a = (float) mraa_aio_read(m_aio);
    if (a == -1.0) return -1;
    // Apply scale factor after error check
    a *= m_scale;
    float r = (float)(1023.0-a)*(float)m_r0/a;
    float t = 1.0/(log(r/(float)m_r0)/(float)m_b + 1.0/298.15)-273.15;
    return (int) round(t);
}

float GroveTemp::raw_value()
{
    return (float) mraa_aio_read(m_aio);
}
