/*
 * Authors: Brendan Le Foll <brendan.le.foll@intel.com>
 *          Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 *          Sarah Knepper <sarah.knepper@intel.com>
 * Copyright (c) 2014 - 2016 Intel Corporation.
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
