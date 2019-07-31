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

#include "rotaryencoder.hpp"

using namespace upm;

RotaryEncoder::RotaryEncoder(int pinA, int pinB) :
    m_rotaryencoder(rotaryencoder_init(pinA, pinB))
{
    if (!m_rotaryencoder)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": rotaryencoder_init failed");
}

RotaryEncoder::~RotaryEncoder()
{
    rotaryencoder_close(m_rotaryencoder);
}

void RotaryEncoder::initPosition(int count)
{
    rotaryencoder_set_position(m_rotaryencoder, count);
}

int RotaryEncoder::position()
{
    return rotaryencoder_get_position(m_rotaryencoder);
}

float RotaryEncoder::getAngle() {
    return (float) RotaryEncoder::position() / 20.0 * 360;
}
