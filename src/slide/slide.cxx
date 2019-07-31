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

#include "slide.hpp"
#include "math.h"

using namespace upm;

Slide::Slide(unsigned int pin, float ref_voltage)
{
    if ( !(m_aio = mraa_aio_init(pin)) ) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_aio_init() failed, invalid pin?");
        return;
    }
    m_ref_voltage = ref_voltage;
}

Slide::~Slide()
{
    mraa_aio_close(m_aio);
}

float Slide::raw_value()
{
    return (float) mraa_aio_read(m_aio);
}

float Slide::voltage_value()
{
    // conversion to Volts
    float a = Slide::raw_value();
    a = m_ref_voltage * a / 1023.0 ;
    return a;
}

float Slide::ref_voltage()
{
    return m_ref_voltage;
}
