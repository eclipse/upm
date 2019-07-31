/*
 * Author: Sarah Knepper <sarah.knepper@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <string>
#include <stdexcept>

#include "ttp223.hpp"

using namespace upm;

TTP223::TTP223(unsigned int pin) {
    // initialize gpio input
    if ( !(m_gpio = mraa_gpio_init(pin)) )
      {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_gpio_init() failed, invalid pin?");
        return;
      }
    mraa_gpio_dir(m_gpio, MRAA_GPIO_IN);
    m_name = "ttp223";
    m_isrInstalled = false;
}

TTP223::~TTP223() {
    // close gpio input
    mraa_gpio_close(m_gpio);
}

std::string TTP223::name() {
    return m_name;
}

int TTP223::value() {
    return mraa_gpio_read(m_gpio);
}

bool TTP223::isPressed() {
    return this->value() == 1;
}


void TTP223::installISR(mraa::Edge level, void (*isr)(void *), void *arg)
{
  if (m_isrInstalled)
    uninstallISR();

  // install our interrupt handler
  mraa_gpio_isr(m_gpio, (mraa_gpio_edge_t) level, isr, arg);
  m_isrInstalled = true;
}

void TTP223::uninstallISR()
{
  mraa_gpio_isr_exit(m_gpio);
  m_isrInstalled = false;
}
