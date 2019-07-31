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

#include "upm_string_parser.hpp"
#include "a110x.hpp"

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

A110X::A110X(std::string initStr) : mraaIo(initStr)
{
  mraa_io_descriptor* descs = mraaIo.getMraaDescriptors();

  if(!descs->gpios)
  {
    throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_gpio_init() failed, invalid pin?");
  }
  else
  {
    if( !(m_gpio = descs->gpios[0]) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                              ": mraa_gpio_init() failed, invalid pin?");

    }
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

void A110X::installISR(void (*isr)(void *), void *arg)
{
  if (m_isrInstalled)
    uninstallISR();

  // install our interrupt handler
  mraa_gpio_isr(m_gpio, MRAA_GPIO_EDGE_BOTH,
                isr, arg);
  m_isrInstalled = true;
}

void A110X::uninstallISR()
{
  mraa_gpio_isr_exit(m_gpio);
  m_isrInstalled = false;
}
