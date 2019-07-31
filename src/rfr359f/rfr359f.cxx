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

#include "rfr359f.hpp"

using namespace upm;
using namespace std;

RFR359F::RFR359F(int pin)
{
  if ( !(m_gpio = mraa_gpio_init(pin)) )
   {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_gpio_init() failed, invalid pin?");
      return;
    }

  mraa_gpio_dir(m_gpio, MRAA_GPIO_IN);
}

RFR359F::~RFR359F()
{
  mraa_gpio_close(m_gpio);
}

bool RFR359F::objectDetected()
{
  return (!mraa_gpio_read(m_gpio) ? true : false);
}
