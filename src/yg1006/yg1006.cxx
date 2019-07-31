/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014-2016 Intel Corporation.
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

#include "yg1006.hpp"

using namespace upm;
using namespace std;

YG1006::YG1006(unsigned int pin) :
    m_yg1006(yg1006_init(pin))
{
    if (!m_yg1006)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": water_init() failed");
}

YG1006::~YG1006()
{
    yg1006_close(m_yg1006);
}

bool YG1006::flameDetected()
{
    return yg1006_flame_detected(m_yg1006);
}
