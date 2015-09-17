/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014 Intel Corporation.
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

#include "enc03r.h"

using namespace upm;
using namespace std;

ENC03R::ENC03R(int pin, float vref)
{
  if ( !(m_aio = mraa_aio_init(pin)) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_aio_init() failed, invalid pin?");
      return;
    }

  m_vref = vref;
  m_calibrationValue = 0;
}

ENC03R::~ENC03R()
{
  mraa_aio_close(m_aio);
}

unsigned int ENC03R::value()
{
  return mraa_aio_read(m_aio);
}

void ENC03R::calibrate(unsigned int samples)
{
  unsigned int val;
  float total = 0.0;

  for (int i=0; i<samples; i++)
    {
      val = mraa_aio_read(m_aio);
      total += (float)val;
      usleep(2000);
    }

  m_calibrationValue = total / (float)samples;
}

double ENC03R::angularVelocity(unsigned int val)
{
  // from seeed studio example
  //return (((double)(val-m_calibrationValue)*(m_vref*1000.0))/1023.0/0.67);
  return (((double)(val-m_calibrationValue)*(m_vref*1000.0))/685.41);
}
