/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
