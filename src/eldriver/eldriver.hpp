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
#pragma once

#include <mraa/gpio.h>

#define HIGH      1
#define LOW       0

namespace upm {
  /**
   * @brief EL Driver Module
   * @defgroup eldriver libupm-eldriver
   * @ingroup seeed gpio electric
   */

  /**
   * @library eldriver
   * @sensor eldriver
   * @comname Electroluminescent Wire (EL) Driver
   * @type electric
   * @man seeed
   * @con gpio
   *
   * @brief API for the  EL Driver Module
   * 
   * The  EL Driver allows you to easily light up an
   * EL wire with just one single  cable.
   *
   * @image html eldriver.jpg 
   * @snippet eldriver.cxx Interesting
   */
  class ElDriver {
  public:
    /**
     *  EL Driver constructor
     *
     * @param pin Digital pin to use
     */
    ElDriver(int pin);
    /**
     *  EL Driver destructor
     */
    ~ElDriver();
    /**
     * Turns the EL wire on
     */
     void on();
    /**
     * Turns the EL wire off
     */
     void off();

  private:
        mraa_gpio_context m_gpio;
	};
}
