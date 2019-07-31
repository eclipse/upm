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

 float Rotary::getAngle()
 {
     return Rotary::abs_deg();
 }
