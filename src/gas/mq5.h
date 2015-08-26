/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * Copyright (c) 2014 Intel Corporation.
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
   * @sensor mq5
   * @comname Grove MQ5 Gas Sensor
   * @type gaseous
   * @man seeed
   * @con analog
   * @kit eak
   *
   * @brief API for the MQ5 Gas Sensor
   *
   * The Grove MQ5 Gas Sensor module is useful for gas leakage detection
   * (in home and industry). It can detect LPG, natural gas, town gas, and so
   * on. It is highly sensitive and has a detection range of
   * 300-10,000 ppm.
   *
   * @image html mq2-5.jpeg
   * @snippet mq5.cxx Interesting
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
