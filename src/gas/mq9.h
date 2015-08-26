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
   * @sensor mq9
   * @comname Grove MQ9 Gas Sensor
   * @type gaseous
   * @man seeed
   * @con analog
   *
   * @brief API for the MQ9 Gas Sensor
   *
   * The Grove MQ9 Gas Sensor module is useful for gas leakage detection (in
   * home and industry). It can detect carbon monoxide, coal gas, and
   * liquefied gas. Its sensitivity is 10-1,000 ppm CO, and 100-10,000 ppm Gas.
   *
   * @image html mq3-9.jpeg
   * @snippet mq9.cxx Interesting
   */
    class MQ9 : public Gas {
        public:
            /**
             * MQ9 constructor
             *
             * @param gasPin Analog pin where the sensor is connected
             */
            MQ9 (int gasPin);

            /**
             * MQ9 destructor
             */
            ~MQ9 ();

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
