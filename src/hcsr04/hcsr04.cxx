/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Author: Rafael Neri <rafael.neri@gmail.com>
 * Copyright (c) 2014-2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <stdexcept>
#include <string>

#include "hcsr04.hpp"

using namespace upm;

HCSR04::HCSR04(int triggerPin, int echoPin) : m_hcsr04(hcsr04_init(triggerPin, echoPin))
{
    if (!m_hcsr04)
        throw std::runtime_error(std::string(__FUNCTION__) + ": hcsr04_init failed");
}

HCSR04::~HCSR04()
{
    hcsr04_close(m_hcsr04);
}

double
HCSR04::getDistance(HCSR04_U unit)
{
    return hcsr04_get_distance(m_hcsr04, unit);
}

float
HCSR04::getDistance()
{
    return getDistance(HCSR04_CM);
}
