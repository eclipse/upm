/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015-2016 Intel Corporation.
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


