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

#include "a110x.h"

using namespace upm;
using namespace std;

A110X::A110X(int pin)
{
  if ( !(m_gpio = mraa_gpio_init(pin)) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_gpio_init() failed, invalid pin?");
      return;
    }

  mraa_gpio_dir(m_gpio, MRAA_GPIO_IN);
  m_isrInstalled = false;
}

A110X::~A110X()
{
  if (m_isrInstalled)
    uninstallISR();

  mraa_gpio_close(m_gpio);
}

bool A110X::magnetDetected()
{
  return (!mraa_gpio_read(m_gpio) ? true : false);
}

#ifdef JAVACALLBACK
void A110X::installISR( IsrCallback *cb)
{
installISR(generic_callback_isr, cb);
}
#endif

void A110X::installISR(void (*isr)(void *), void *arg)
{
  if (m_isrInstalled)
    uninstallISR();

  // install our interrupt handler
  mraa_gpio_isr(m_gpio, MRAA_GPIO_EDGE_FALLING, 
                isr, arg);
  m_isrInstalled = true;
}

void A110X::uninstallISR()
{
  mraa_gpio_isr_exit(m_gpio);
  m_isrInstalled = false;
}
