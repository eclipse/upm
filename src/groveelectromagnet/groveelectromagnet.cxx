/*
 * Author: Zion Orent <sorent@ics.com>
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

#include <string>
#include <stdexcept>

#include "groveelectromagnet.h"

using namespace upm;

GroveElectromagnet::GroveElectromagnet(int pin)
{
    if ( !(m_gpio = mraa_gpio_init(pin)) ) 
      {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_gpio_init() failed, invalid pin?");
        return;
      }
    mraa_gpio_dir(m_gpio, MRAA_GPIO_OUT);
}

GroveElectromagnet::~GroveElectromagnet()
{
    mraa_gpio_close(m_gpio);
}

void GroveElectromagnet::on()
{
	mraa_result_t error = MRAA_SUCCESS;
	error = mraa_gpio_write (m_gpio, HIGH);
	if (error != MRAA_SUCCESS)
		mraa_result_print(error);
}

void GroveElectromagnet::off()
{
	mraa_result_t error = MRAA_SUCCESS;
	error = mraa_gpio_write (m_gpio, LOW);
	if (error != MRAA_SUCCESS)
		mraa_result_print(error);
}
