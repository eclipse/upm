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

#include "collision.hpp"
#include "upm_string_parser.hpp"

using namespace upm;

Collision::Collision(int pin)
{
    if ( !(m_gpio = mraa_gpio_init(pin)) )
      {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_gpio_init() failed, invalid pin?");
        return;
      }
    mraa_gpio_dir(m_gpio, MRAA_GPIO_IN);
}

Collision::Collision(std::string initStr) : mraaIo(initStr)
{
    mraa_io_descriptor* descs = mraaIo.getMraaDescriptors();
    if(!descs->gpios) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_gpio_init() failed, invalid pin?");
    } else {
      if( !(m_gpio = descs->gpios[0]) ) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_gpio_init() failed, invalid pin?");
      }
    }
}

Collision::~Collision()
{
    mraa_gpio_close(m_gpio);
}

bool Collision::isColliding()
{
	// Collisions cause 0; no collision is 1
	return (!(bool)mraa_gpio_read(m_gpio));
}
