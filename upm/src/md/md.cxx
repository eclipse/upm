/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014-2016 Intel Corporation.
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

#include <unistd.h>
#include <iostream>
#include <string>
#include <stdexcept>

#include "md.hpp"

using namespace upm;
using namespace std;


MD::MD(int bus, uint8_t address) :
    m_md(md_init(bus, address))
{
    if (!m_md)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": md_init() failed");
}

MD::~MD()
{
    md_close(m_md);
}

bool MD::writePacket(MD_REG_T reg, uint8_t data1, uint8_t data2)
{
    if (!md_write_packet(m_md, reg, data1, data2))
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": md_write_packet() failed");

    return true;
}

bool MD::setMotorSpeeds(uint8_t speedA, uint8_t speedB)
{
    if (!md_set_motor_speeds(m_md, speedA, speedB))
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": md_set_motor_speeds() failed");

    return true;
}

bool MD::setPWMFrequencyPrescale(uint8_t freq)
{
    if (!md_set_pwm_frequency_prescale(m_md, freq))
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": md_set_pwm_frequency_prescale() failed");

    return true;
}

bool MD::setMotorDirections(MD_DC_DIRECTION_T dirA, MD_DC_DIRECTION_T dirB)
{
    if (!md_set_motor_directions(m_md, dirA, dirB))
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": md_set_motor_directions() failed");

    return true;
}

bool MD::enableStepper(MD_STEP_DIRECTION_T dir, uint8_t speed)
{
    return md_enable_stepper(m_md, dir, speed);
}

bool MD::disableStepper()
{
    return md_disable_stepper(m_md);
}

bool MD::setStepperSteps(unsigned int steps)
{
    return md_set_stepper_steps(m_md, steps);
}

void MD::configStepper(unsigned int stepsPerRev, MD_STEP_MODE_T mode)
{
    md_config_stepper(m_md, stepsPerRev, mode);
}

