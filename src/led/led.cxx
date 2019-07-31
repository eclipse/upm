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

#include "led.hpp"

using namespace std;
using namespace upm;

Led::Led(int pin)
{
    if ( !(m_led = led_init(pin)) ) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_gpio_init() failed, invalid pin?");
        return;
    }
}

Led::Led(std::string name)
{
    if ( !(m_led = led_init_str(name.c_str())) ) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_led_init() failed, invalid led name?");
        return;
    }
}

Led::~Led()
{
    led_close(m_led);
}

upm_result_t Led::write(int value)
{
    if (value >= 1) {
        return led_on(m_led);
    }
    return led_off(m_led);
}

upm_result_t Led::on()
{
    return write(1);
}

upm_result_t Led::off()
{
    return write(0);
}
