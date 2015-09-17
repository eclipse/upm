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

#include "groveehr.h"

using namespace upm;
using namespace std;

GroveEHR::GroveEHR(int pin)
{
  if ( !(m_gpio = mraa_gpio_init(pin)) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_aio_init() failed, invalid pin?");
      return;
    }

  mraa_gpio_dir(m_gpio, MRAA_GPIO_IN);

  initClock();
  m_beatCounter = 0;
}

GroveEHR::~GroveEHR()
{
  mraa_gpio_close(m_gpio);
}

void GroveEHR::initClock()
{
  gettimeofday(&m_startTime, NULL);
}

uint32_t GroveEHR::getMillis()
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

void GroveEHR::clearBeatCounter()
{
  m_beatCounter = 0;
}

void GroveEHR::startBeatCounter()
{
  // install our interrupt handler
  mraa_gpio_isr(m_gpio, MRAA_GPIO_EDGE_RISING, 
                &beatISR, this);
}

void GroveEHR::stopBeatCounter()
{
  // remove the interrupt handler
  mraa_gpio_isr_exit(m_gpio);
}

uint32_t GroveEHR::beatCounter()
{
  return m_beatCounter;
}

void GroveEHR::beatISR(void *ctx)
{
  upm::GroveEHR *This = (upm::GroveEHR *)ctx;
  This->m_beatCounter++;
}

int GroveEHR::heartRate()
{
  uint32_t millis = getMillis();
  uint32_t beats = beatCounter();
  
  float heartRate = 0;
  // wait at least 5 seconds before attempting to compute the
  // heart rate
  if (millis > 5000)
    {
      heartRate = (float(beats) / (float(millis) / 1000.0)) * 60.0;
    }

  return int(heartRate);
}
