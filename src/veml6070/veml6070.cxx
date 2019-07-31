/*
 * Author: Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2017 Intel Corporation.
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
