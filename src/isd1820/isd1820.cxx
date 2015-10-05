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
#include <string>
#include <stdexcept>

#include "isd1820.h"

using namespace upm;
using namespace std;

ISD1820::ISD1820(int playPin, int recPin)
{
  if ( !(m_gpioPlay = mraa_gpio_init(playPin)) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_gpio_init(playPin) failed, invalid pin?");
      return;
    }

  mraa_gpio_dir(m_gpioPlay, MRAA_GPIO_OUT);

  if ( !(m_gpioRec = mraa_gpio_init(recPin)) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_gpio_init(recPin) failed, invalid pin?");
      return;
    }

  mraa_gpio_dir(m_gpioRec, MRAA_GPIO_OUT);

  // make sure we are not recording or playing
  play(false);
  record(false);
}

ISD1820::~ISD1820()
{
  mraa_gpio_close(m_gpioPlay);
  mraa_gpio_close(m_gpioRec);
}

void ISD1820::play(bool enable)
{
  if (enable)
    {
      // make sure we are not recording
      record(false);
      mraa_gpio_write(m_gpioPlay, 1);
    }
  else
    {
      mraa_gpio_write(m_gpioPlay, 0);
    }
}

void ISD1820::record(bool enable)
{
  if (enable)
    {
      // make sure we are not playing
      play(false);
      mraa_gpio_write(m_gpioRec, 1);
    }
  else
    {
      mraa_gpio_write(m_gpioRec, 0);
    }
}
