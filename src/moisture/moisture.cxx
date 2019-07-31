/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014 Intel Corporation.
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

#include "moisture.hpp"

using namespace upm;

Moisture::Moisture(int pin)
{
  if ( !(m_aio = mraa_aio_init(pin)) )
    throw std::invalid_argument(std::string(__FUNCTION__) +
                                ": mraa_aio_init() failed, invalid pin?");
}

Moisture::~Moisture()
{
  mraa_aio_close(m_aio);
}

int Moisture::value()
{
  return mraa_aio_read(m_aio);
}

int Moisture::getMoisture()
{
  return value();
}
