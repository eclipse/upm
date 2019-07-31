/*
 * Authors: Brendan Le Foll <brendan.le.foll@intel.com>
 *          Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 *          Sarah Knepper <sarah.knepper@intel.com>
 * Copyright (c) 2014 - 2016 Intel Corporation.
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

#include "button.hpp"

using namespace std;
using namespace upm;

Button::Button(unsigned int pin)
{
    if ( !(m_gpio = mraa_gpio_init(pin)) ) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_gpio_init() failed, invalid pin?");
        return;
    }
    mraa_gpio_dir(m_gpio, MRAA_GPIO_IN);
    m_name = "Button Sensor";
}

Button::Button(std::string initStr) : mraaIo(initStr)
{
    mraa_io_descriptor* descs = mraaIo.getMraaDescriptors();
    if(!descs->gpios) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_gpio_init() failed, invalid pin?");
    } else {
      if( !(m_gpio = descs->gpios[0]) ) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_gpio_init() failed, invalid pin?");
      }
    }

    mraa_gpio_dir(m_gpio, MRAA_GPIO_IN);
    m_name = "Button Sensor";
}

Button::~Button()
{
    mraa_gpio_close(m_gpio);
}

std::string Button::name()
{
    return m_name;
}

int Button::value()
{
    return mraa_gpio_read(m_gpio);
}

bool Button::isPressed()
{
    return (bool) Button::value();
}

void Button::installISR(mraa::Edge level, void (*isr)(void *), void *arg)
{
  if (m_isrInstalled)
    uninstallISR();

  // install our interrupt handler
  mraa_gpio_isr(m_gpio, (mraa_gpio_edge_t) level, isr, arg);
  m_isrInstalled = true;
}

void Button::uninstallISR()
{
  mraa_gpio_isr_exit(m_gpio);
  m_isrInstalled = false;
}
