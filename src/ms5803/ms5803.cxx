/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
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
    return ms5803_get_pressure(m_ms5803);
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
