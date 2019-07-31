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

