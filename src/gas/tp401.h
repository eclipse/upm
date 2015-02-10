/*
 * Author: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
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
     * @brief C++ API for Grove TP401 Air Quality Sensor
     *
     * The Grove TP401 Air Quality Sensor module is useful for monitoring air purity indoors.
     * It can detect CO and a wide range of other harmful gases, but due to limited sensing
     * range should be used only when qualitative results are needed. Example applications
     * are air recirculation, ventilation systems, and refreshing sprayers.
     * The sensor is linear and should be roughly sensitive to 0 ~ 20 ppm CO from 0 ~ 4V.
     * Also note that the sensor requires 2-3 minutes to warm up initially and 48 hours of
     * operation to stabilize completely.
     *
     * @ingroup gas analog
     * @snippet tp401.cxx Interesting
     * @image html tp401.jpeg
     */
    class TP401 : public Gas {
        public:
            /**
             * TP401 constructor
             *
             * @param gasPin analog pin where sensor was connected
             */
            TP401 (int gasPin);

            /**
             * TP401 destructor
             */
            ~TP401 ();

            /**
             * Return name of the component
             *
             * @return a string with the name of the sensor
             */
            std::string name()
            {
                return m_name;
            }

            /**
             * Returns one sample in parts per million (ppm) of CO in the air based on
             * the following sensor calibration: 0 ~ 4V is roughly 0 ~ 20 ppm CO
             *
             * @return a new sample converted to ppm CO
             */
            float getPPM();

        private:
            std::string m_name;
    };
}
