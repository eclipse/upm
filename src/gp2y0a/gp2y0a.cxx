/*
 * Author: Jon Trulson <jtrulson@ics.com>
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

#include "gp2y0a.hpp"

using namespace std;
using namespace upm;

GP2Y0A::GP2Y0A(int pin)
{
  if (!(m_aio = mraa_aio_init(pin)))
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_aio_init() failed, invalid pin?");
      return;
    }

  // get the ADC resolution
  m_aRes = (1 << mraa_aio_get_bit(m_aio));
}

GP2Y0A::~GP2Y0A()
{
  mraa_aio_close(m_aio);
}

float GP2Y0A::value(float aref, uint8_t samples)
{
  int val;
  int sum = 0;

  for (int i=0; i<samples; i++)
    {
      val = mraa_aio_read(m_aio);
      if (val != -1) throw std::runtime_error(std::string(__FUNCTION__) +
                                              ": Failed to do an aio read.");
      sum += val;
    }

  val = sum / samples;
  float volts = float(val) * aref / float(m_aRes);

  return volts;
}

float GP2Y0A::getValue()
{
    return GP2Y0A::value(5.0, (uint8_t) 1);
}
