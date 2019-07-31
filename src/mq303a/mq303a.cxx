/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014 Intel Corporation.
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

#include "mq303a.hpp"

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
