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

#include "uln200xa.hpp"

using namespace upm;
using namespace std;

ULN200XA::ULN200XA(int stepsPerRev, unsigned int i1, unsigned int i2,
                   unsigned int i3, unsigned int i4) :
    m_uln200xa(uln200xa_init(stepsPerRev, i1, i2, i3, i4))
{
    if (!m_uln200xa)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": uln200xa_init() failed");
}

ULN200XA::~ULN200XA()
{
    uln200xa_close(m_uln200xa);
}

void ULN200XA::setSpeed(unsigned int speed)
{
    uln200xa_set_speed(m_uln200xa, speed);
}

void ULN200XA::setDirection(ULN200XA_DIRECTION_T dir)
{
    uln200xa_set_direction(m_uln200xa, dir);
}

void ULN200XA::stepperSteps(unsigned int steps)
{
    uln200xa_stepper_steps(m_uln200xa, steps);
}

void ULN200XA::release()
{
    uln200xa_release(m_uln200xa);
}
