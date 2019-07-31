/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <iostream>
#include <string>
#include "gas.hpp"

namespace upm {
  /**
   * @library gas
   * @sensor mq9
   * @comname MQ9 Carbon Monoxide (CO) and Flammable Gas Sensor
   * @type gaseous
   * @man seeed
   * @con analog
   * @web http://wiki.seeed.cc/Grove-Gas_Sensor-MQ9/
   *
   * @brief API for the MQ9 Gas Sensor
   *
   * The Grove MQ9 Gas Sensor module is useful for gas leakage detection (in
   * home and industry). It can detect carbon monoxide, coal gas, and
   * liquefied gas. Its sensitivity is 10-1,000 ppm CO, and 100-10,000 ppm Gas.
   *
   * @image html mq3-9.jpeg
   * @snippet gas-mq9.cxx Interesting
   */
    class MQ9 : public Gas {
        public:
            /**
             * MQ9 constructor
             *
             * @param gasPin Analog pin where the sensor is connected
             */
            MQ9 (int gasPin);

            /**
             * MQ9 destructor
             */
            ~MQ9 ();

            /**
             * Returns the name of the sensor
             */
            std::string name()
            {
                return m_name;
            }
        private:
            std::string m_name;
    };
}
