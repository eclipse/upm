/*
 * Authors: Brendan Le Foll <brendan.le.foll@intel.com>
 *          Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 *          Sarah Knepper <sarah.knepper@intel.com>
 * Copyright (c) 2014 - 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string>
#include <mraa/gpio.hpp>

namespace upm {
 /**
  * @brief Relay Library
  * @defgroup relaylib libupm-relay
  * @ingroup seeed gpio relay gsk eak hak
  */

 /**
  * @library relaylib
  * @sensor relay
  * @comname Relay
  * @altname Grove Relay
  * @type relay
  * @man seeed
  * @con gpio
  * @kit gsk eak hak
  *
  * @brief API for the Relay
  *
  * UPM module for the relay switch. The relay is a digital normally-open
  * switch that uses low voltage or current to control a higher voltage and/or
  * higher current.  When closed, the indicator LED (if present) lights up and
  * current is allowed to flow.
  *
  * @image html relay.jpg
  * @snippet relay.cxx Interesting
  */

  class Relay{
    public:
        /**
         * relay constructor
         *
         * @param pin Pin to use
         */
        Relay(unsigned int pin);
        /**
         * relay destructor
         */
        ~Relay();
        /**
         * Sets the relay switch to on (closed). This allows current
         * to flow and lights up the indicator LED.
         *
         * @return 0 if successful, non-zero otherwise
         */
        mraa_result_t on();
        /**
         * Sets the relay switch to off (open). This stops current
         * from flowing and the indicator LED is not lit.
         *
         * @return 0 if successful, non-zero otherwise
         */
        mraa_result_t off();
        /**
         * Defines whether the relay switch is closed.
         *
         * @return True if the switch is on (closed), false otherwise
         */
        bool isOn();
        /**
         * Defines whether the relay switch is open.
         *
         * @return True if the switch is off (open), false otherwise
         */
        bool isOff();
        std::string name(){ return "Relay Switch";}
    private:
        mraa_gpio_context m_gpio;
  };
}
