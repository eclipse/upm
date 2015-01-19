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

#include "rotaryencoder.h"

using namespace upm;
using namespace std;

RotaryEncoder::RotaryEncoder(int pinA, int pinB)
{
  if ( !(m_gpioA = mraa_gpio_init(pinA)) )
    {
      cerr << __FUNCTION__ << ": mraa_gpio_init() failed" << endl;
      return;
    }

  mraa_gpio_dir(m_gpioA, MRAA_GPIO_IN);

  if ( !(m_gpioB = mraa_gpio_init(pinB)) )
    {
      cerr << __FUNCTION__ << ": mraa_gpio_init() failed" << endl;
      return;
    }

  mraa_gpio_dir(m_gpioB, MRAA_GPIO_IN);

  m_position = 0;

  // setup the ISR

  // We would prefer to use MRAA_GPIO_EDGE_BOTH for better resolution,
  // but that does not appear to be supported
  mraa_gpio_isr(m_gpioA, MRAA_GPIO_EDGE_RISING, 
                &signalAISR, this);
}

RotaryEncoder::~RotaryEncoder()
{
  mraa_gpio_isr_exit(m_gpioA);

  mraa_gpio_close(m_gpioA);
  mraa_gpio_close(m_gpioB);
}

void RotaryEncoder::initPosition(int count)
{
  m_position = count;
}

int RotaryEncoder::position()
{
  return m_position;
}

void RotaryEncoder::signalAISR(void *ctx)
{
  upm::RotaryEncoder *This = (upm::RotaryEncoder *)ctx;

  if (mraa_gpio_read(This->m_gpioA))
    {
      if (mraa_gpio_read(This->m_gpioB))
        This->m_position++;      // CW
      else
        This->m_position--;      // CCW
    }
}


