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

#include "grovevdiv.h"

using namespace upm;
using namespace std;

GroveVDiv::GroveVDiv(int pin)
{
  if ( !(m_aio = mraa_aio_init(pin)) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_aio_init() failed, invalid pin?");
      return;
    }
}

GroveVDiv::~GroveVDiv()
{
  mraa_aio_close(m_aio);
}

unsigned int GroveVDiv::value(unsigned int samples)
{
  unsigned int sum = 0;

  for (int i=0; i<samples; i++)
    {
      sum += mraa_aio_read(m_aio);
      usleep(2000);
    }
        
  return (sum / samples);
}

float GroveVDiv::computedValue(uint8_t gain, unsigned int val, int vref, int res)
{
  return ((float(gain) * float(val) * float(vref) / float(res)) / 1000.0);

}

