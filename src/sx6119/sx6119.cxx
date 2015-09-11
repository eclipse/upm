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

#include <unistd.h>
#include <iostream>
#include <string>
#include <stdexcept>

#include "sx6119.h"

using namespace upm;
using namespace std;

SX6119::SX6119(int powerPin, int seekPin)
{
  if ( !(m_gpioPower = mraa_gpio_init(powerPin)) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_gpio_init(power) failed, invalid pin?");
      return;
    }

  mraa_gpio_dir(m_gpioPower, MRAA_GPIO_OUT);
  mraa_gpio_write(m_gpioPower, 1);

  if ( !(m_gpioSeek = mraa_gpio_init(seekPin)) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_gpio_init(seek) failed, invalid pin?");
      return;
    }

  mraa_gpio_dir(m_gpioSeek, MRAA_GPIO_OUT);
  mraa_gpio_write(m_gpioSeek, 1);
}

SX6119::~SX6119()
{
  mraa_gpio_close(m_gpioPower);
  mraa_gpio_close(m_gpioSeek);
}

void SX6119::togglePower()
{
  // this is just a toggle -- we set LOW for one second and power will
  // be turned on or off depending on the previous condition.
  mraa_gpio_write(m_gpioPower, 0);
  sleep(1);
  mraa_gpio_write(m_gpioPower, 1);
}

void SX6119::seek()
{
  // this is just a trigger -- we set LOW for 500ms to seek to the
  // next available station, wrapping around when we reach the end.
  mraa_gpio_write(m_gpioSeek, 0);
  usleep(500000);
  mraa_gpio_write(m_gpioSeek, 1);
}
