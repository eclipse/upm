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

#include "vdiv.hpp"

using namespace upm;
using namespace std;

VDiv::VDiv(int pin)
{
  if ( !(m_aio = mraa_aio_init(pin)) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_aio_init() failed, invalid pin?");
      return;
    }
}

VDiv::~VDiv()
{
  mraa_aio_close(m_aio);
}

unsigned int VDiv::value(unsigned int samples)
{
  int sum = 0;

  for (unsigned int i=0; i<samples; i++)
    {
      sum += mraa_aio_read(m_aio);
      if (sum == -1) return 0;
      usleep(2000);
    }

  return (sum / samples);
}

unsigned int VDiv::getValue()
{
    return VDiv::value(1);
}

float VDiv::computedValue(uint8_t gain, unsigned int val, int vref, int res)
{
  return ((float(gain) * float(val) * float(vref) / float(res)) / 1000.0);

}
