/*
 * Author: Zion Orent <zorent@ics.com>
 * Copyright (c) 2015 Intel Corporation.
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

#include "apds9002.hpp"
#include "upm_string_parser.hpp"

using namespace upm;

APDS9002::APDS9002(int pin)
{
  if ( !(m_aio = mraa_aio_init(pin)) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_aio_init() failed, invalid pin?");
      return;
    }
}

APDS9002::APDS9002(std::string initStr) : mraaIo(initStr)
{
  mraa_io_descriptor* descs = mraaIo.getMraaDescriptors();

  if(!descs->aios)
  {
    throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_aio_init() failed, invalid pin?");
  }
  else
  {
    if( !(m_aio = descs->aios[0]) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                              ": mraa_aio_init() failed, invalid pin?");

    }
  }
}

APDS9002::~APDS9002()
{
  mraa_aio_close(m_aio);
}

int APDS9002::value()
{
  return mraa_aio_read(m_aio);
}

float APDS9002::getLuminance()
{
    return value();
}
