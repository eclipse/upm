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
#include <interfaces/iAngle.hpp>

namespace upm {

/**
 * @deprecated This class is being replaced by the upm-rotary library and the
 * Rotary class.
 * @library grove
 * @sensor groverotary
 * @comname Rotary Angle Sensor
 * @altname Rotary Potentiometer
 * @type ainput
 * @man seeed
 * @con analog
 * @kit gsk
 *
 * @brief API for the Grove Rotary Angle Sensor (Knob)
 *
 * Basic UPM module for the Grove rotary angle sensor (knob) on analog. Provides
 * a set of functions to read the absolute pin value, degrees or radians, and another set
 * to do the same relative to the center of the knob's range.
 *
 * @image html rotaryencoder.jpg
 * @snippet grove-groverotary.cxx Interesting
 */
class GroveRotary: public Grove, virtual public iAngle {
    public:
        /**
         * Grove rotary angle sensor constructor
         *
         * @param pin Number of the analog pin to use
         */
        GroveRotary(unsigned int pin);
        /**
         * GroveRotary destructor
         */
        ~GroveRotary();
        /**
         * Gets the absolute raw value from the AIO pin
         *
         * @return Unsigned value from the ADC
         */
        float abs_value();
        /**
         * Gets absolute raw degrees from the AIO pin
         *
         * @return Unsigned degrees from the ADC
         */
        float abs_deg();
        /**
         * Gets absolute raw radians from the AIO pin
         *
         * @return Unsigned radians from the ADC
         */
        float abs_rad();
        /**
         * Gets the relative value from the AIO pin
         *
         * @return Signed value from the ADC
         */
        float rel_value();
        /**
         * Gets relative degrees from the AIO pin
         *
         * @return Signed degrees from the ADC
         */
        float rel_deg();
        /**
         * Gets relative radians from the AIO pin
         *
         * @return Signed radians from the ADC
         */
        float rel_rad();

        /**
         * Get rotation value as raw degrees from the AIO pin.
         *
         * @return rotation value.
         */
        virtual float getAngle();

    private:
        mraa_aio_context m_aio;
        static const int m_max_angle = 300;
};
}
