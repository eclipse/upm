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
   * @sensor mq7
   * @comname MQ7 Carbon Monoxide Sensor
   * @type gaseous
   * @man dfrobot
   * @con analog
   * @web https://www.dfrobot.com/wiki/index.php/Carbon_Monoxide_Gas_Sensor(MQ7)_(SKU:SEN0132)
   *
   * @brief API for the MQ7 CO Gas Sensor
   *
   * The Grove MQ7 Gas Sensor module is useful for detecting Carbon
   * Monoxide (CO) concentrations in the air.  It has a detection
   * range of 20-2000 ppm.
   *
   * For optimum use, it requires calibration after a pre-heat time of
   * 48 hours.  See the datasheet for details.
   *
   * @image html mq3-9.jpeg
   * @snippet gas-mq7.cxx Interesting
   */
    class MQ7 : public Gas {
        public:
            /**
             * MQ7 constructor
             *
             * @param gasPin Analog pin where the sensor is connected
             */
            MQ7 (int gasPin);

            /**
             * MQ7 destructor
             */
            ~MQ7 ();

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
