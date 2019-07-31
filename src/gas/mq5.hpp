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
   * @sensor mq5
   * @comname MQ5 Natural Gas and Liquefied Petroleum Gas (LPG) Sensor
   * @type gaseous
   * @man seeed
   * @con analog
   * @web http://wiki.seeed.cc/Grove-Gas_Sensor-MQ5/
   * @kit eak
   *
   * @brief API for the MQ5 Gas Sensor
   *
   * The MQ5 Gas Sensor module is useful for gas leakage detection
   * (in home and industry). It can detect LPG, natural gas, town gas, and so
   * on. It is highly sensitive and has a detection range of
   * 300-10,000 ppm.
   *
   * @image html mq2-5.jpeg
   * @snippet gas-mq5.cxx Interesting
   */
    class MQ5 : public Gas {
        public:
            /**
             * MQ5 constructor
             *
             * @param gasPin Analog pin where the sensor is connected
             */
            MQ5 (int gasPin);

            /**
             * MQ9 destructor
             */
            ~MQ5 ();

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
