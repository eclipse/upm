/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * This driver was rewritten based on the original driver written by:
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 *
 * The MIT License
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
#include <stdlib.h>

#include "bmpx8x.hpp"

using namespace upm;
using namespace std;

BMPX8X::BMPX8X (int bus, int addr) :
    m_bmpx8x(bmpx8x_init(bus, addr))
{
    if (!m_bmpx8x)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmpx8x_init() failed");
}

BMPX8X::~BMPX8X()
{
    bmpx8x_close(m_bmpx8x);
}

void BMPX8X::init(BMPX8X_OSS_T oss)
{
    if (bmpx8x_devinit(m_bmpx8x, oss))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmpx8x_devinit() failed");
}

void BMPX8X::update()
{
    if (bmpx8x_update(m_bmpx8x))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmpx8x_update() failed");
}

void BMPX8X::reset()
{
    if (bmpx8x_reset(m_bmpx8x))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmpx8x_reset() failed");
}

void BMPX8X::setOversampling(BMPX8X_OSS_T oss)
{
    bmpx8x_set_oversampling(m_bmpx8x, oss);
}

uint8_t BMPX8X::readReg(uint8_t reg)
{
    return bmpx8x_read_reg(m_bmpx8x, reg);
}

int BMPX8X::readRegs(uint8_t reg, uint8_t *buffer, int len)
{
    int rv = bmpx8x_read_regs(m_bmpx8x, reg, buffer, len);
    if (rv < 0)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmpx8x_read_regs() failed");

    return rv;
}

void BMPX8X::writeReg(uint8_t reg, uint8_t val)
{
    if (bmpx8x_write_reg(m_bmpx8x, reg, val))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmpx8x_write_reg() failed");
}

int BMPX8X::getPressure()
{
    return bmpx8x_get_pressure(m_bmpx8x);
}

float BMPX8X::getTemperature()
{
    return bmpx8x_get_temperature(m_bmpx8x);
}

int BMPX8X::getSealevelPressure(float altitudeMeters)
{
    return bmpx8x_get_sealevel_pressure(m_bmpx8x, altitudeMeters);
}

float BMPX8X::getAltitude(int sealevelPressure)
{
    return bmpx8x_get_altitude(m_bmpx8x, sealevelPressure);
}
