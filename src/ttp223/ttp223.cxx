/*
 * Author: Sarah Knepper <sarah.knepper@intel.com>
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

#include "ttp223.h"

using namespace upm;

TTP223::TTP223(unsigned int pin) {
    // initialize gpio input
    mraa_init();
    m_gpio = mraa_gpio_init(pin);
    mraa_gpio_dir(m_gpio, MRAA_GPIO_IN);
    m_name = "ttp223";
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
