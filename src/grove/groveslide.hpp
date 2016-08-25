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
 * @sensor groveslide
 * @comname Grove Slide Potentiometer
 * @type ainput
 * @man seeed
 * @con analog
 *
 * @brief API for the Grove Slide Potentiometer
 *
 * Basic UPM module for the Grove slide potentiometer on analog that
 * returns either a raw value or a scaled voltage value.
 *
 * @image html groveslide.jpeg
 * @snippet groveslide.cxx Interesting
 */
class GroveSlide: public Grove {
    public:
        /**
         * Grove analog slide potentiometer constructor
         *
         * @param pin Number of the analog pin to use
         *
         * @param ref_voltage Reference voltage the board is set to, as a floating-point value; default is 5.0V
         */
        GroveSlide(unsigned int pin, float ref_voltage = 5.0);
        /**
         * GroveSlide destructor
         */
        ~GroveSlide();
        /**
         * Gets the raw value from the AIO pin
         *
         * @return Raw value from the ADC
         */
        float raw_value();
        /**
         * Gets the voltage value from the pin
         *
         * @return Voltage reading based on the reference voltage
         */
        float voltage_value();
        /**
         * Gets the board's reference voltage passed on object initialization
         *
         * @return Reference voltage the class was set for
         */
        float ref_voltage();
    private:
        mraa_aio_context m_aio;
        float m_ref_voltage;
};
}
