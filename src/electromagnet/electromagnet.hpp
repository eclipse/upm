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
#include <interfaces/iElectromagnet.hpp>

#define HIGH      1
#define LOW       0

namespace upm {
  /**
   * @brief Electromagnet Library
   * @defgroup electromagnet libupm-electromagnet
   * @ingroup seeed gpio electric
   */

  /**
   * @library electromagnet
   * @sensor electromagnet
   * @comname Electromagnet
   * @type electric
   * @man seeed
   * @con gpio
   *
   * @brief API for the  Electromagnet
   *
   * The  Electromagnet can hold up to 1 kg (approximately 2.2 lbs)
   *
   * @image html electromagnet.jpg
   * @snippet electromagnet.cxx Interesting
   */
  class Electromagnet : virtual public iElectromagnet {
  public:
    /**
     *  Electromagnet constructor
     *
     * @param pin Digital pin to use
     */
    Electromagnet(int pin);
    /**
     *  Electromagnet destructor
     */
    ~Electromagnet();
    /**
     * Turns the magnet on
     */
     virtual void on();
    /**
     * Turns the magnet off
     */
     virtual void off();

  private:
        mraa_gpio_context m_gpio;
	};
}
