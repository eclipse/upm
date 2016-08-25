/*
 * Authors: Brendan Le Foll <brendan.le.foll@intel.com>
 *          Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 *          Sarah Knepper <sarah.knepper@intel.com>
 * Copyright (c) 2014 - 2016 Intel Corporation.
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

#include <string>
#include <mraa/aio.hpp>
#include "grovebase.hpp"

namespace upm {

/**
 * @library grove
 * @sensor grovetemp
 * @comname Grove Temperature Sensor
 * @type temp
 * @man seeed
 * @con analog
 * @kit gsk
 *
 * @brief API for the Grove Temperature Sensor
 *
 * Basic UPM module for the Grove temperature sensor on analog. This sensor
 * uses a thermistor to measure ambient temperature. The conversion formula has
 * been updated to work with versions 1.1 and 1.2 of the sensor. For the older
 * v1.0 sensor you will have to specify R0 and B values when initializing the
 * device. The range of this sensor is -40 to 125 C and accuracy is +/- 1.5 C.
 *
 * @image html grovetemp.jpg
 * @snippet grovetemp.cxx Interesting
 */
class GroveTemp: public Grove {
    public:
        /**
         * Grove analog temperature sensor constructor
         *
         * @param pin Analog pin to use
         * @param scale Scaling factor for raw analog value from the ADC,
         * useful for mixed 3.3V/5V boards, default 1.0
         * @param r0 zero power resistance, this is 100K (default) for
         * v1.1-v1.2 and 10K for v1.0 of the sensor
         * @param b thermistor nominal B constant, this is 4275 (default) for
         * v1.1-v1.2 and 3975 for v1.0 of the sensor
         */
        GroveTemp(unsigned int pin, float scale = 1.0, int r0 = 100000, int b = 4275);
        /**
         * GroveTemp destructor
         */
        ~GroveTemp();
        /**
         * Gets the raw value from the AIO pin
         *
         * @return Raw value from the ADC
         */
        float raw_value();
        /**
         * Gets the temperature in Celsius from the sensor
         *
         * @return Normalized temperature in Celsius
         */
        int value();
    private:
        mraa_aio_context m_aio;
        float m_scale;
        int m_r0;
        int m_b;

};
}
