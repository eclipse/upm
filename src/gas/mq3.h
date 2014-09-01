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
     * @brief C++ API for MQ3 gas sensor
     *
     * The Grove MQ3 Gas Sensor module is useful for gas leakage detecting (in
     * home and industry). It can detect Alcohol Vapour and Benzine. It's
     * highly sensitive but has a long warmup time of about 1minute. It's
     * detect rate is of 0.04-4mg/L Alcohol.
     *
     * @snippet mq3-example.cxx Interesting
     * @image html grovegas.jpeg
     */
    class MQ3 : public Gas {
        public:
            /**
             * MQ3 constructor
             *
             * @param gasPin analog pin where sensor connected
             */
            MQ3 (int gasPin);

            /**
             * MQ3 destructor
             */
            ~MQ3 ();

            /**
             * Return name of the component
             */
            std::string name()
            {
                return m_name;
            }
        private:
            std::string m_name;
    };
}
