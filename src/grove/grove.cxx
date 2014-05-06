/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * Copyright (c) 2014 Intel Corporation.
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

#include "grove.h"

using namespace upm;

GroveLed::GroveLed(int pin)
{
    maa_init();
    m_gpio = maa_gpio_init(pin);
    maa_gpio_dir(m_gpio, MAA_GPIO_OUT);
    m_name = "LED Socket";
}

GroveLed::~GroveLed()
{
    maa_gpio_close(m_gpio);
}

maa_result_t GroveLed::write(int value)
{
    if (value >= 1) {
        return maa_gpio_write(m_gpio, 1);
    }
    return maa_gpio_write(m_gpio, 0);
}

maa_result_t GroveLed::on()
{
    return write(1);
}

maa_result_t GroveLed::off()
{
    return write(0);
}
