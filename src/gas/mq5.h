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
     * @brief C++ API for MQ5 gas sensor
     *
     * The Grove - Gas Sensor (MQ5) module is useful for gas leakage detecting
     * (in home and industry). It can detect LPG, natural gas, town gas and so
     * on. It is highly sensitive and has a detection concentration of
     * 300-10000 ppm.
     *
     * @ingroup gas analog
     * @snippet mq5.cxx Interesting
     * @image html mq2-5.jpeg
     */
    class MQ5 : public Gas {
        public:
            /**
             * MQ5 constructor
             *
             * @param gasPin analog pin where sensor is connected
             */
            MQ5 (int gasPin);

            /**
             * MQ9 destructor
             */
            ~MQ5 ();

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
