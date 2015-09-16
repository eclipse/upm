/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * Contributions: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 * Contributions: Sarah Knepper <sarah.knepper@intel.com>
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
#include <string>
#include <stdexcept>

#include "grove.h"
#include "math.h"

using namespace upm;

//// GroveLed ////

GroveLed::GroveLed(int pin)
{
    if ( !(m_gpio = mraa_gpio_init(pin)) ) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_gpio_init() failed, invalid pin?");
        return;
    }
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

//// GroveRelay ////

GroveRelay::GroveRelay(unsigned int pin)
{
    if ( !(m_gpio = mraa_gpio_init(pin)) ) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_gpio_init() failed, invalid pin?");
        return;
    }
    mraa_gpio_dir(m_gpio, MRAA_GPIO_OUT);
    m_name = "Relay Switch";
}

GroveRelay::~GroveRelay()
{
    mraa_gpio_close(m_gpio);
}

mraa_result_t GroveRelay::on()
{
    return mraa_gpio_write(m_gpio, 1);
}

mraa_result_t GroveRelay::off()
{
    return mraa_gpio_write(m_gpio, 0);
}

bool GroveRelay::isOn()
{
    return mraa_gpio_read(m_gpio) == 1;
}

bool GroveRelay::isOff()
{
    return mraa_gpio_read(m_gpio) == 0;
}

//// GroveTemp ////

GroveTemp::GroveTemp(unsigned int pin)
{
    if ( !(m_aio = mraa_aio_init(pin)) ) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_aio_init() failed, invalid pin?");
        return;
    }
    m_name = "Temperature Sensor";
}

GroveTemp::~GroveTemp()
{
    mraa_aio_close(m_aio);
}

int GroveTemp::value ()
{
    int a = mraa_aio_read(m_aio);
    float r = (float)(1023.0-a)*10000.0/a;
    float t = 1.0/(log(r/10000.0)/3975.0 + 1.0/298.15)-273.15;
    return (int) round(t);
}

float GroveTemp::raw_value()
{
    return (float) mraa_aio_read(m_aio);
}

//// GroveLight ////

GroveLight::GroveLight(unsigned int pin)
{
    if ( !(m_aio = mraa_aio_init(pin)) ) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_aio_init() failed, invalid pin?");
        return;
    }
    m_name = "Light Sensor";
}

GroveLight::~GroveLight()
{
    mraa_aio_close(m_aio);
}

int GroveLight::value()
{
    // rough conversion to lux, using formula from Grove Starter Kit booklet
    float a = (float) mraa_aio_read(m_aio);
    a = 10000.0/pow(((1023.0-a)*10.0/a)*15.0,4.0/3.0);
    return (int) round(a);
}

float GroveLight::raw_value()
{
    return (float) mraa_aio_read(m_aio);
}

//// GroveRotary ////

GroveRotary::GroveRotary(unsigned int pin)
{
    if ( !(m_aio = mraa_aio_init(pin)) ) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_aio_init() failed, invalid pin?");
        return;
    }
    m_name = "Rotary Angle Sensor";
}

GroveRotary::~GroveRotary()
{
    mraa_aio_close(m_aio);
}

float GroveRotary::abs_value()
{
    return (float) mraa_aio_read(m_aio);
}

float GroveRotary::abs_deg()
{
    return GroveRotary::abs_value() * (float) m_max_angle / 1023.0;
}

float GroveRotary::abs_rad()
{
    return GroveRotary::abs_deg() * M_PI / 180.0;
}

float GroveRotary::rel_value()
{
    return GroveRotary::abs_value() - 512.0;
}

float GroveRotary::rel_deg()
{
    return GroveRotary::rel_value() * (float) m_max_angle / 1023.0;
}

float GroveRotary::rel_rad()
{
    return GroveRotary::rel_deg() * M_PI / 180.0;
}

//// GroveSlide ////

GroveSlide::GroveSlide(unsigned int pin, float ref_voltage)
{
    if ( !(m_aio = mraa_aio_init(pin)) ) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_aio_init() failed, invalid pin?");
        return;
    }
    m_ref_voltage = ref_voltage;
    m_name = "Slide Potentiometer";
}

GroveSlide::~GroveSlide()
{
    mraa_aio_close(m_aio);
}

float GroveSlide::raw_value()
{
    return (float) mraa_aio_read(m_aio);
}

float GroveSlide::voltage_value()
{
    // conversion to Volts
    float a = GroveSlide::raw_value();
    a = m_ref_voltage * a / 1023.0 ;
    return a;
}

float GroveSlide::ref_voltage()
{
    return m_ref_voltage;
}

//// GroveButton ////

GroveButton::GroveButton(unsigned int pin)
{
    if ( !(m_gpio = mraa_gpio_init(pin)) ) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_gpio_init() failed, invalid pin?");
        return;
    }
    mraa_gpio_dir(m_gpio, MRAA_GPIO_IN);
    m_name = "Button Sensor";
}

GroveButton::~GroveButton()
{
    mraa_gpio_close(m_gpio);
}

std::string GroveButton::name()
{
    return m_name;
}

int GroveButton::value()
{
    return mraa_gpio_read(m_gpio);
}

#ifdef JAVACALLBACK
void GroveButton::installISR(mraa::Edge level, IsrCallback *cb)
{
  installISR(level, generic_callback_isr, cb);
}
#endif

void GroveButton::installISR(mraa::Edge level, void (*isr)(void *), void *arg)
{
  if (m_isrInstalled)
    uninstallISR();

  // install our interrupt handler
  mraa_gpio_isr(m_gpio, (mraa_gpio_edge_t) level, isr, arg);
  m_isrInstalled = true;
}

void GroveButton::uninstallISR()
{
  mraa_gpio_isr_exit(m_gpio);
  m_isrInstalled = false;
}
