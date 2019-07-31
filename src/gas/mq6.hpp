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
   * @sensor mq6
   * @comname MQ6 Liquefied Petroleum Gas (LPG) and Butane Sensor
   * @type gaseous
   * @man dfrobot
   * @con analog
   * @web https://www.dfrobot.com/wiki/index.php/LPG_Gas_Sensor(MQ6)_(SKU:SEN0131)
   *
   * @brief API for the MQ6 LPG Gas Sensor
   *
   * The MQ6 Gas Sensor module is useful for detecting LPG,
   * iso-butane, propane, and LNG concentrations in the air.  It has a
   * detection range of 200-10000 ppm.
   *
   * For optimum use, it requires calibration after a pre-heat time of
   * at least 24 hours.  See the datasheet for details.
   *
   * @image html mq2-5.jpeg
   * @snippet gas-mq6.cxx Interesting
   */
    class MQ6 : public Gas {
        public:
            /**
             * MQ6 constructor
             *
             * @param gasPin Analog pin where the sensor is connected
             */
            MQ6 (int gasPin);

            /**
             * MQ6 destructor
             */
            ~MQ6 ();

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
