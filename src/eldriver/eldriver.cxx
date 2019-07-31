/*
 * Author: Zion Orent <sorent@ics.com>
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

#include "eldriver.hpp"

using namespace upm;

ElDriver::ElDriver(int pin)
{
    if ( !(m_gpio = mraa_gpio_init(pin)) ) 
      {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_gpio_init() failed, invalid pin?");
        return;
      }
    mraa_gpio_dir(m_gpio, MRAA_GPIO_OUT);
}

ElDriver::~ElDriver()
{
    mraa_gpio_close(m_gpio);
}

void ElDriver::on()
{
	mraa_result_t error = MRAA_SUCCESS;
	error = mraa_gpio_write (m_gpio, HIGH);
	if (error != MRAA_SUCCESS)
		mraa_result_print(error);
}

void ElDriver::off()
{
	mraa_result_t error = MRAA_SUCCESS;
	error = mraa_gpio_write (m_gpio, LOW);
	if (error != MRAA_SUCCESS)
		mraa_result_print(error);
}
