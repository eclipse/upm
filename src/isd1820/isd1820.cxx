/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <string>
#include <stdexcept>

#include "isd1820.hpp"

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
