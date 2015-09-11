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

#include "ta12200.h"

using namespace upm;
using namespace std;

TA12200::TA12200(int pin)
{
  initClock();

  if ( !(m_aio = mraa_aio_init(pin)) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_aio_init() failed, invalid pin?");
      return;
    }
}

TA12200::~TA12200()
{
  mraa_aio_close(m_aio);
}

void TA12200::initClock()
{
  gettimeofday(&m_startTime, NULL);
}

uint32_t TA12200::getMillis()
{
  struct timeval elapsed, now;
  uint32_t elapse;

  // get current time
  gettimeofday(&now, NULL);

  // compute the delta since m_startTime
  if( (elapsed.tv_usec = now.tv_usec - m_startTime.tv_usec) < 0 ) 
    {
      elapsed.tv_usec += 1000000;
      elapsed.tv_sec = now.tv_sec - m_startTime.tv_sec - 1;
    } 
  else 
    {
      elapsed.tv_sec = now.tv_sec - m_startTime.tv_sec;
    }

  elapse = (uint32_t)((elapsed.tv_sec * 1000) + (elapsed.tv_usec / 1000));

  // never return 0
  if (elapse == 0)
    elapse = 1;

  return elapse;
}


unsigned int TA12200::highestValue()
{
  unsigned int hiVal = 0;
  unsigned int val;
  uint32_t start = getMillis();

  // 1 second
  while (getMillis() < (start + 1000))
    {
      val = mraa_aio_read(m_aio);
      if (val > hiVal)
        hiVal = val;
    }
        
  return hiVal;
}

float TA12200::milliAmps(unsigned int val, int res)
{
  float ampCurrent;
  float effectiveVal;

  // From grove wiki page:
  // minimum_current=1/1024*5/800*2000000/1.414=8.6(mA) 
  // Only for sinusoidal alternating current
  //ampCurrent = float(val) / float(res) * 5.0 / 800.0 * 2000000.0;
  ampCurrent = float(val) / float(res) * 12500.0;
  effectiveVal = ampCurrent/1.414;
  return (effectiveVal);
}

