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

#include "relay.hpp"

using namespace upm;

Relay::Relay(unsigned int pin)
{
    if ( !(m_gpio = mraa_gpio_init(pin)) ) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_gpio_init() failed, invalid pin?");
        return;
    }
    mraa_gpio_dir(m_gpio, MRAA_GPIO_OUT);
}

Relay::~Relay()
{
    mraa_gpio_close(m_gpio);
}

mraa_result_t Relay::on()
{
    return mraa_gpio_write(m_gpio, 1);
}

mraa_result_t Relay::off()
{
    return mraa_gpio_write(m_gpio, 0);
}

bool Relay::isOn()
{
    return mraa_gpio_read(m_gpio) == 1;
}

bool Relay::isOff()
{
    return mraa_gpio_read(m_gpio) == 0;
}
