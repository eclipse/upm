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
   * @sensor mq3
   * @comname MQ3 Alcohol, Ethanol, Smoke Sensor
   * @type gaseous
   * @man seeed
   * @con analog
   * @web http://wiki.seeed.cc/Grove-Gas_Sensor-MQ3/
   *
   * @brief API for the MQ3 Gas Sensor
   *
   * The MQ3 Gas Sensor module is useful for gas leakage detection (in
   * home and industry). It can detect alcohol vapors and benzine. It's
   * highly sensitive but has a long warm-up time of about 1 minute. It's
   * detection range is 0.04-4 mg/L Alcohol.
   *
   * @image html mq3-9.jpeg
   * @snippet gas-mq3.cxx Interesting
   */
    class MQ3 : public Gas {
        public:
            /**
             * MQ3 constructor
             *
             * @param gasPin Analog pin where the sensor is connected
             */
            MQ3 (int gasPin);

            /**
             * MQ3 destructor
             */
            ~MQ3 ();

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
