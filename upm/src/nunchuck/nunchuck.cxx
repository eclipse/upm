/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015-2017 Intel Corporation.
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

