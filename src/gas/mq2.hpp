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
   * @sensor mq2
   * @comname MQ2 Methane, Butane, Liquefied Petroleum Gas (LPG), and Smoke Sensor
   * @type gaseous
   * @man seeed
   * @con analog
   * @kit hak
   * @web http://wiki.seeed.cc/Grove-Gas_Sensor-MQ2/
   *
   * @brief API for the MQ2 Gas Sensor
   *
   * The MQ2 Gas Sensor module is useful for gas leakage detection
   * (in home and industry). It can detect LPG, i-butane, methane, alcohol,
   * hydrogen, smoke, and other combustible gases. It's a medium-sensitivity
   * sensor with a detection range of 300-10,000 ppm.
   *
   * @image html mq2-5.jpeg
   * @snippet gas-mq2.cxx Interesting
   */
    class MQ2 : public Gas {
        public:
            /**
             * MQ2 constructor
             *
             * @param gasPin Analog pin where the sensor is connected
             */
            MQ2 (int gasPin);

            /**
             * MQ2 destructor
             */
            ~MQ2 ();

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
