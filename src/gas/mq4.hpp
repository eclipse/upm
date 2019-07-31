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
   * @sensor mq4
   * @comname MQ4 Methane and Connecticut Natural Gas (CNG) Sensor
   * @type gaseous
   * @man dfrobot
   * @con analog
   * @web https://www.dfrobot.com/index.php?route=product/product&product_id=683
   *
   * @brief API for the MQ4 Gas Sensor
   *
   * The MQ4 Gas Sensor module is useful for detecting CH4 (Methane),
   * and natural gas concentrations in the air.  It has a
   * detection range of 100-10000 ppm.
   *
   * For optimum use, it requires calibration after a pre-heat time of
   * at least 24 hours.  See the datasheet for details.
   *
   * @image html mq2-5.jpeg
   * @snippet gas-mq4.cxx Interesting
   */
    class MQ4 : public Gas {
        public:
            /**
             * MQ4 constructor
             *
             * @param gasPin Analog pin where the sensor is connected
             */
            MQ4 (int gasPin);

            /**
             * MQ4 destructor
             */
            ~MQ4 ();

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
