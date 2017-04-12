/*
 * Author: Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2017 Intel Corporation.
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

#include "abp.hpp"

using namespace upm;

ABP::ABP(int bus, int devAddress) :
    m_abp(abp_init(bus, devAddress))
{
    if(!m_abp)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                ": abp_init failed");
}

ABP::~ABP()
{
    abp_close(m_abp);
}

void ABP::update()
{
    if(abp_update(m_abp) != UPM_SUCCESS)
        std::cout << " ABP sensor unable to update data" << std::endl;
}

float ABP::getPressure()
{
    return abp_get_pressure(m_abp);
}

float ABP::getTemperature()
{
    return abp_get_temperature(m_abp);
}

void ABP::setMaxPressure(int max)
{
    abp_set_max_pressure(m_abp, max);
}

void ABP::setMinPressure(int min)
{
    abp_set_min_pressure(m_abp, min);
}
