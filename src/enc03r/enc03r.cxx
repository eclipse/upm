/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014-2017 Intel Corporation.
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

#include "enc03r.hpp"

using namespace upm;
using namespace std;

ENC03R::ENC03R(int pin, float aref) :
    m_enc03r(enc03r_init(pin, aref))
{
    if (!m_enc03r)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": enc03r_init() failed");
}

ENC03R::~ENC03R()
{
    enc03r_close(m_enc03r);
}

void ENC03R::calibrate(unsigned int samples)
{
    if (enc03r_calibrate(m_enc03r, samples))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": enc03r_calibrate() failed");
}

void ENC03R::update()
{
    if (enc03r_update(m_enc03r))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": enc03r_update() failed");
}

float ENC03R::angularVelocity()
{
    return enc03r_angular_velocity(m_enc03r);
}

void ENC03R::setOffset(float offset)
{
    enc03r_set_offset(m_enc03r, offset);
}

void ENC03R::setScale(float scale)
{
    enc03r_set_scale(m_enc03r, scale);
}

float ENC03R::getNormalized()
{
    return enc03r_get_normalized(m_enc03r);
}
