/*
 * Author: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
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
   * @sensor tp401
   * @comname TP401 Gas Sensor
   * @altname Grove Air Quality Sensor
   * @type gaseous
   * @man seeed
   * @con analog
   * @web https://www.seeedstudio.com/Grove-Air-quality-sensor-p-1065.html
   * @kit hak
   *
   * @brief API for the TP401 Gas Sensor
   *
   * The TP401 Gas Sensor module is useful for monitoring air purity indoors.
   * It can detect CO and a wide range of other harmful gases, but, due to a limited detection
   * range, it should be used only when qualitative results are needed. Example applications
   * are air recirculation, ventilation systems, and refreshing sprayers.
   * The sensor is linear and should be roughly sensitive to 0-20 ppm CO from 0-4 V.
   * Note: the sensor requires 2-3 minutes to warm up initially and 48 hours of
   * operation to stabilize completely.
   *
   * @image html tp401.jpeg
   * @snippet gas-tp401.cxx Interesting
   */
    class TP401 : public Gas {
        public:
            /**
             * TP401 constructor
             *
             * @param gasPin Analog pin where the sensor is connected
             */
            TP401 (int gasPin);

            /**
             * TP401 destructor
             */
            ~TP401 ();

            /**
             * Returns the name of the sensor
             *
             * @return Name of the sensor
             */
            std::string name()
            {
                return m_name;
            }

            /**
             * Returns one sample in parts per million (ppm) of CO in the air based on
             * the following sensor calibration: 0-4 V is roughly 0-20 ppm CO
             *
             * @return  New sample converted to ppm CO
             */
            float getPPM();

        private:
            std::string m_name;
    };
}
