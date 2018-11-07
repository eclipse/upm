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

#include "rotary.hpp"
#include "math.h"

using namespace upm;

Rotary::Rotary(unsigned int pin)
{
    if ( !(m_aio = mraa_aio_init(pin)) ) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_aio_init() failed, invalid pin?");
        return;
    }
}

Rotary::~Rotary()
{
    mraa_aio_close(m_aio);
}

float Rotary::abs_value()
{
    return (float) mraa_aio_read(m_aio);
}

float Rotary::abs_deg()
{
    return Rotary::abs_value() * (float) m_max_angle / 1023.0;
}

float Rotary::abs_rad()
{
    return Rotary::abs_deg() * M_PI / 180.0;
}

float Rotary::rel_value()
{
    return Rotary::abs_value() - 512.0;
}

float Rotary::rel_deg()
{
    return Rotary::rel_value() * (float) m_max_angle / 1023.0;
}

float Rotary::rel_rad()
{
    return Rotary::rel_deg() * M_PI / 180.0;
}
