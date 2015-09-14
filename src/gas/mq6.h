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
#include "gas.h"

namespace upm {
  /**
   * @library gas
   * @sensor mq6
   * @comname DFRobot MQ6 LPG Gas Sensor
   * @type gaseous
   * @man dfrobot
   * @con analog
   *
   * @brief API for the DFRobot MQ6 LPG Gas Sensor
   *
   * The MQ6 Gas Sensor module is useful for detecting LPG,
   * iso-butane, propane, and LNG concentrations in the air.  It has a
   * detection range of 200-10000 ppm.
   *
   * For optimum use, it requires calibration after a pre-heat time of
   * at least 24 hours.  See the datasheet for details.
   *
   * @image html mq2-5.jpeg
   * @snippet mq6.cxx Interesting
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
