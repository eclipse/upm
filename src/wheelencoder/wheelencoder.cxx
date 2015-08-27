/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
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

#include "wheelencoder.h"

using namespace upm;
using namespace std;

WheelEncoder::WheelEncoder(int pin) :
  m_gpio(pin)
{
  m_gpio.dir(mraa::DIR_IN);

  initClock();
  m_counter = 0;
  m_isrInstalled = false;
}

WheelEncoder::~WheelEncoder()
{
  stopCounter();
}

void WheelEncoder::initClock()
{
  gettimeofday(&m_startTime, NULL);
}

uint32_t WheelEncoder::getMillis()
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

void WheelEncoder::startCounter()
{
  initClock();
  m_counter = 0;

  // install our interrupt handler
  if (!m_isrInstalled)
    m_gpio.isr(mraa::EDGE_RISING, &wheelISR, this);

  m_isrInstalled = true;
}

void WheelEncoder::stopCounter()
{
  // remove the interrupt handler
  if (m_isrInstalled)
    m_gpio.isrExit();

  m_isrInstalled = false;
}

void WheelEncoder::wheelISR(void *ctx)
{
  upm::WheelEncoder *This = (upm::WheelEncoder *)ctx;
  This->m_counter++;
}

