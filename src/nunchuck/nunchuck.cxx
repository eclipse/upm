/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015-2017 Intel Corporation.
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
#include <unistd.h>

#include "nunchuck.hpp"

using namespace upm;
using namespace std;


NUNCHUCK::NUNCHUCK(int bus) :
    m_nunchuck(nunchuck_init(bus))
{
    if (!m_nunchuck)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": nunchuck_init() failed");
}

NUNCHUCK::~NUNCHUCK()
{
    nunchuck_close(m_nunchuck);
}

void NUNCHUCK::update()
{
    if (nunchuck_update(m_nunchuck))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": nunchuck_update() failed");

    nunchuck_get_stick(m_nunchuck, &stickX, &stickY);
    nunchuck_get_acceleration(m_nunchuck, &accelX, &accelY, &accelZ);
    nunchuck_get_buttons(m_nunchuck, &buttonC, &buttonZ);
}

