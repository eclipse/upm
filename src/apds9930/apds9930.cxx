/*
 * Author: Lay, Kuan Loon <kuan.loon.lay@intel.com>
 * Copyright (c) 2016 Intel Corporation.
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

#include "apds9930.hpp"
#include "upm_string_parser.hpp"

using namespace upm;

APDS9930::APDS9930(int device)
{
    if (!(m_iio = mraa_iio_init(device))) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_iio_init() failed, invalid device?");
        return;
    }
}

APDS9930::APDS9930(std::string initStr) : mraaIo(initStr)
{
    mraa_io_descriptor* descs = mraaIo.getMraaDescriptors();

    if(!descs->iios)
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_iio_init() failed");
    }
    else
    {
        if(!(m_iio = descs->iios[0]))
        {
            throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_iio_init() failed");

        }
    }

    std::vector<std::string> upmTokens;

    if (!mraaIo.getLeftoverStr().empty()) {
        upmTokens = UpmStringParser::parse(mraaIo.getLeftoverStr());
    }

    for (std::string tok : upmTokens) {
      if(tok.substr(0, 16) == "enableProximity:") {
          bool enable = std::stoi(tok.substr(16), nullptr, 0);
          enableProximity(enable);
      }
      if(tok.substr(0, 18) == "enableIlluminance:") {
          bool enable = std::stoi(tok.substr(18), nullptr, 0);
          enableIlluminance(enable);
      }
    }
}


APDS9930::~APDS9930()
{
    if (m_iio)
        mraa_iio_close(m_iio);
}

int
APDS9930::getAmbient()
{
    int iio_value = 0;
    mraa_iio_read_int(m_iio, "in_illuminance_input", &iio_value);
    return iio_value;
}

int
APDS9930::getProximity()
{
    int iio_value = 0;
    mraa_iio_read_int(m_iio, "in_proximity_raw", &iio_value);
    return iio_value;
}

bool
APDS9930::enableProximity(bool enable)
{
    if (enable)
        mraa_iio_write_int(m_iio, "in_proximity_en", 1);
    else
        mraa_iio_write_int(m_iio, "in_proximity_en", 0);

    return true;
}

bool
APDS9930::enableIlluminance(bool enable)
{
    if (enable)
        mraa_iio_write_int(m_iio, "in_illuminance_en", 1);
    else
        mraa_iio_write_int(m_iio, "in_illuminance_en", 0);

    return true;
}
