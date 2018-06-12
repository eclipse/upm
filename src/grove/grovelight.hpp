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
#include <interfaces/iLight.hpp>
#include "grovebase.hpp"

namespace upm {

/**
 * @deprecated This class is being replaced by the upm-light library and the
 * Light class.
 * @library grove
 * @sensor grovelight
 * @comname Light Sensor
 * @type light
 * @man seeed
 * @con analog
 * @kit gsk
 *
 * @brief API for the Grove Light Sensor
 *
 * The Grove light sensor detects the intensity of the ambient light.
 * As the light intensity of the environment increases, the resistance
 * of the sensor decreases. This means the raw value from the
 * analog pin is larger in bright light and smaller in the dark.
 * An approximate lux value can also be returned.
 *
 * @image html grovelight.jpg
 * @snippet grove-grovelight.cxx Interesting
 */
class GroveLight: public Grove, virtual public iLight {
    public:
        /**
         * Grove analog light sensor constructor
         *
         * @param pin Analog pin to use
         */
        GroveLight(unsigned int pin);
        /**
         * GroveLight destructor
         */
        ~GroveLight();
        /**
         * Gets the raw value from the AIO pin
         *
         * @return Raw value from the ADC
         */
        float raw_value();
        /**
         * Gets an approximate light value, in lux, from the sensor
         *
         * @return Normalized light reading in lux
         */
        int value();

        /**
         * Gets an approximate light value, in lux, from the sensor
         *
         * @return Normalized light reading in lux
         */
        virtual float getLuminance();
    private:
        mraa_aio_context m_aio;
};
}
