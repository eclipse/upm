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
#include "waterlevel.hpp"

using namespace upm;

WaterLevel::WaterLevel(int pin)
{
    if ( !(m_gpio = mraa_gpio_init(pin)) ) 
      {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_gpio_init() failed, invalid pin?");
        return;
      }
    mraa_gpio_dir(m_gpio, MRAA_GPIO_IN);
}

WaterLevel::~WaterLevel()
{
    mraa_gpio_close(m_gpio);
}

bool WaterLevel::isSubmerged()
{
	// Submerged causes 0; being above water is 1
	return (!(bool)mraa_gpio_read(m_gpio));
}
