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

#include <iostream>

#include "grove.h"
#include "math.h"

using namespace upm;

//// GroveLed ////

GroveLed::GroveLed(int pin)
{
    mraa_init();
    m_gpio = mraa_gpio_init(pin);
    mraa_gpio_dir(m_gpio, MRAA_GPIO_OUT);
    m_name = "LED Socket";
}

GroveLed::~GroveLed()
{
    mraa_gpio_close(m_gpio);
}

mraa_result_t GroveLed::write(int value)
{
    if (value >= 1) {
        return mraa_gpio_write(m_gpio, 1);
    }
    return mraa_gpio_write(m_gpio, 0);
}

mraa_result_t GroveLed::on()
{
    return write(1);
}

mraa_result_t GroveLed::off()
{
    return write(0);
}

//// GroveTemp ////

GroveTemp::GroveTemp(unsigned int pin)
{
    mraa_init();
    m_aio = mraa_aio_init(pin);
    m_name = "Temperature Sensor";
}

GroveTemp::~GroveTemp()
{
    mraa_aio_close(m_aio);
}

int GroveTemp::value ()
{
    int a = mraa_aio_read(m_aio);
    float r = (float)(1023-a)*10000/a;
    float t = 1/(logf(r/10000)/3975 + 1/298.15)-273.15;
    return (int) t;
}

float GroveTemp::raw_value()
{
    return (float) mraa_aio_read(m_aio);
}

//// GroveLight ////

GroveLight::GroveLight(unsigned int pin)
{
    mraa_init();
    m_aio = mraa_aio_init(pin);
    m_name = "Light Sensor";
}

GroveLight::~GroveLight()
{
    mraa_aio_close(m_aio);
}

int GroveLight::value ()
{
    // rough conversion to Lux
    int a = mraa_aio_read(m_aio);
    a = 10000/(((1023-a)*10/a)*15)^(4/3);
    return a;
}

float GroveLight::raw_value()
{
    return (float) mraa_aio_read(m_aio);
}
