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
