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

#include "abpdrrt005pg2a5.hpp"

using namespace upm;

ABPDRRT005PG2A5::ABPDRRT005PG2A5(int bus, int devAddress) :
    m_abpdrrt005pg2a5(abpdrrt005pg2a5_init(bus, devAddress))
{
    if(!m_abpdrrt005pg2a5)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                ": abpdrrt005pg2a5_init failed");
}

ABPDRRT005PG2A5::~ABPDRRT005PG2A5()
{
    abpdrrt005pg2a5_close(m_abpdrrt005pg2a5);
}

float ABPDRRT005PG2A5::get_pressure_psi()
{
    float psi_pressure;
	if(abpdrrt005pg2a5_get_pressure_psi(m_abpdrrt005pg2a5, &psi_pressure) != UPM_SUCCESS) {
        throw std::runtime_error(std::string(__FUNCTION__) +
                                ": abpdrrt005pg2a5_get_pressure_psi unable to get " + 
                                "pressure from sensor");
    }

    return psi_pressure;
}

float ABPDRRT005PG2A5::get_pressure_pascal()
{
    float pascal_pressure;
    if(abpdrrt005pg2a5_get_pressure_pascal(m_abpdrrt005pg2a5, &pascal_pressure) != UPM_SUCCESS) {
        throw std::runtime_error(std::string(__FUNCTION__) +
                                ": abpdrrt005pg2a5_get_pressure_pascal unable to get " + 
                                "pressure from sensor");
    }

    return pascal_pressure;
}