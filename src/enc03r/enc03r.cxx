/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014-2017 Intel Corporation.
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
