/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <stdexcept>

#include "mb704x.hpp"

using namespace upm;
using namespace std;

MB704X::MB704X(unsigned int bus, unsigned int addr) :
    m_mb704x(mb704x_init(bus, addr))
{
    if (!m_mb704x)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": mb704x_*_init() failed");
}

MB704X::~MB704X()
{
    mb704x_close(m_mb704x);
}

int MB704X::getRange()
{
    return mb704x_get_range(m_mb704x);
}

float MB704X::getDistance()
{
    return getRange();
}
