/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <unistd.h>
#include <iostream>
#include <stdexcept>
#include <string>

#include "ms5803.hpp"

using namespace upm;
using namespace std;

MS5803::MS5803(int bus, int address, int csPin) :
    m_ms5803(ms5803_init(bus, address, csPin))
{
    if (!m_ms5803)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": ms5803_init() failed");
}

MS5803::~MS5803()
{
    ms5803_close(m_ms5803);
}

void MS5803::update()
{
    if (ms5803_update(m_ms5803))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": ms5803_update() failed");
}

void MS5803::reset()
{
    if (ms5803_reset(m_ms5803))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": ms5803_reset() failed");
}

void MS5803::setTemperatureOSR(MS5803_OSR_T osr)
{
    ms5803_set_temperature_osr(m_ms5803, osr);
}

void MS5803::setPressureOSR(MS5803_OSR_T osr)
{
    ms5803_set_pressure_osr(m_ms5803, osr);
}


float MS5803::getTemperature()
{
    return ms5803_get_temperature(m_ms5803);
}

float MS5803::getPressure()
{
    return (ms5803_get_pressure(m_ms5803) * 100);
}

void MS5803::loadCoefficients()
{
    if (ms5803_load_coefficients(m_ms5803))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": ms5803_load_coefficients() failed");
}

string MS5803::busRead(int cmd, int len)
{
    uint8_t ucmd = (uint8_t)(cmd & 0xff);
    uint8_t ulen = (uint8_t)(len & 0xff);

    uint8_t *data = new uint8_t[ulen];

    if (ms5803_bus_read(m_ms5803, ucmd, data, ulen))
    {
        delete [] data;
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": ms5803_bus_read() failed");
    }

    string dataStr((char *)data, ulen);
    delete [] data;

    return dataStr;
}

void MS5803::busWrite(int cmd, string data)
{
    uint8_t ucmd = (uint8_t)(cmd & 0xff);

    if (ms5803_bus_write(m_ms5803, ucmd, (uint8_t *)data.data(),
                         (uint8_t)data.size()))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": ms5803_bus_write() failed");
}
