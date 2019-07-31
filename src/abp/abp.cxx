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

#include "upm_string_parser.hpp"
#include "abp.hpp"

using namespace upm;

ABP::ABP(int bus, int devAddress) :
    m_abp(abp_init(bus, devAddress))
{
    if(!m_abp)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                ": abp_init failed");
}

ABP::ABP(std::string initStr) : mraaIo(initStr)
{
    mraa_io_descriptor* descs = mraaIo.getMraaDescriptors();
    std::vector<std::string> upmTokens;

    if(!mraaIo.getLeftoverStr().empty())
    {
        upmTokens = UpmStringParser::parse(mraaIo.getLeftoverStr());
    }

    m_abp = (abp_context)malloc(sizeof(struct _abp_context));

    if(!m_abp)
    {
        throw std::runtime_error(std::string(__FUNCTION__) +
                                ": abp_init failed");
    }

    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
       throw std::runtime_error(std::string(__FUNCTION__) +
                                ": mraa_init() failed");
    }

    if(!descs->i2cs)
    {
      abp_close(m_abp);
      throw std::runtime_error(std::string(__FUNCTION__) +
                                ": mraa_i2c_init() failed");
    }
    else
    {
        if( !(m_abp->i2c = descs->i2cs[0]) )
        {
            abp_close(m_abp);
            throw std::runtime_error(std::string(__FUNCTION__) +
                                ": mraa_i2c_init() failed");
        }
    }

    // setting up defaults
    m_abp->abp_pressure_max = 5;
    m_abp->abp_pressure_min = 0;

    for (std::string tok : upmTokens) {
      if(tok.substr(0, 12) == "maxPressure:") {
          int pmax = std::stoi(tok.substr(12), nullptr, 0);
          setMaxPressure(pmax);
      }
      if(tok.substr(0, 12) == "minPressure:") {
          int pmin  = std::stoi(tok.substr(12), nullptr, 0);
          setMinPressure(pmin);
      }
    }
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
