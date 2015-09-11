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

#include "mq303a.h"

using namespace upm;

MQ303A::MQ303A(int pin, int heaterPin)
{
  if ( !(m_aio = mraa_aio_init(pin)) ) 
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_aio_init() failed, invalid pin?");
      return;
    }

  if ( !(m_gpio = mraa_gpio_init(heaterPin)) ) 
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_gpio_init() failed, invalid pin?");
      return;
    }
  mraa_gpio_dir(m_gpio, MRAA_GPIO_OUT);
}

MQ303A::~MQ303A()
{
  heaterEnable(false);
  mraa_aio_close(m_aio);
  mraa_gpio_close(m_gpio);
}

int MQ303A::value()
{
  return (1023 - mraa_aio_read(m_aio));
}

void MQ303A::heaterEnable(bool enable)
{
  if (enable)
    mraa_gpio_write(m_gpio, 0);  // 0 turns on the heater
  else
    mraa_gpio_write(m_gpio, 1);  // 1 turns off the heater
}
