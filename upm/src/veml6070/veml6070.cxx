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

#include "veml6070.hpp"

using namespace upm;

VEML6070::VEML6070(int bus) :
    m_veml6070(veml6070_init(bus))
{
    if(!m_veml6070)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                ": veml6070_init failed");
}

VEML6070::~VEML6070()
{
    veml6070_close(m_veml6070);
}

int VEML6070::getUVIntensity()
{
    return veml6070_get_uv_intensity(m_veml6070);
}

void VEML6070::setIntegrationTime(veml6070_integration_time_t time)
{
    if(veml6070_set_integration_time(m_veml6070, time) != UPM_SUCCESS)
        std::cout << "VEML6070 sensor unable to update integration time" << std::endl;
}
