/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
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
   * @sensor mq8
   * @comname MQ8 Flammable (Hydrogen) Gas Sensor
   * @type gaseous
   * @man dfrobot
   * @con analog
   * @web https://www.dfrobot.com/wiki/index.php/Hydrogen_Gas_Sensor(MQ8)_(SKU:SEN0133)
   *
   * @brief API for the MQ8 Hydrogen (H2) Gas Sensor
   *
   * The MQ8 Gas Sensor module is useful for detecting Hydrogen gas
   * concentrations in the air.  It has a detection range of 100-10000
   * ppm.
   *
   * For optimum use, it requires calibration after a pre-heat time of
   * at least 24 hours.  See the datasheet for details.
   *
   * @image html mq2-5.jpeg
   * @snippet gas-mq8.cxx Interesting
   */
    class MQ8 : public Gas {
        public:
            /**
             * MQ8 constructor
             *
             * @param gasPin Analog pin where the sensor is connected
             */
            MQ8 (int gasPin);

            /**
             * MQ8 destructor
             */
            ~MQ8 ();

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
