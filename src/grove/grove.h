/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * Contributions: Mihai Tudor Panu <mihai.t.panu@intel.com>
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

#include <string>
#include <mraa/aio.h>
#include <mraa/gpio.h>

namespace upm {

/**
 * @brief library for basic grove sensors
 * @defgroup grove libupm-grove
 */
class Grove {
    public:
        virtual ~Grove() {}
        std::string name()
        {
            return m_name;
        }
    protected:
        std::string m_name;
};

/**
 * @brief C++ API for Grove LED
 *
 * Very basic UPM module for grove LED, or any LED for that matter
 *
 * @ingroup grove gpio
 * @snippet groveled.cxx Interesting
 */
class GroveLed: public Grove {
    public:
        GroveLed(int pin);
        ~GroveLed();
        mraa_result_t write(int value);
        mraa_result_t off();
        mraa_result_t on();
    private:
        mraa_gpio_context m_gpio;
};

/**
 * @brief C++ API for Grove Temperature sensor
 *
 * Very basic UPM module for grove temperature sensor on analog
 *
 * @ingroup grove analog
 * @snippet grovetemp.cxx Interesting
 */
class GroveTemp: public Grove {
    public:
        /**
         * Grove analog temperature sensor constructor
         *
         * @param analog pin to use
         */
        GroveTemp(unsigned int pin);
        /**
         * GroveTemp destructor
         */
        ~GroveTemp();
        /**
         * Get raw value from AIO pin
         *
         * @return the raw value from the ADC
         */
        float raw_value();
        /**
         * Get the temperature from the sensor
         *
         * @return the normalised temperature
         */
        int value();
    private:
        mraa_aio_context m_aio;
};

/**
 * @brief C++ API for Grove light sensor
 *
 * Very basic UPM module for grove Light sensor on analog
 *
 * @ingroup grove analog
 * @snippet grovelight.cxx Interesting
 */
class GroveLight: public Grove {
    public:
        /**
         * Grove analog light sensor constructor
         *
         * @param analog pin to use
         */
        GroveLight(unsigned int pin);
        /**
         * GroveLight Destructor
         */
        ~GroveLight();
        /**
         * Get raw value from AIO pin
         *
         * @return the raw value from the ADC
         */
        float raw_value();
        /**
         * Get the light value from the sensor
         *
         * @return the normalised light reading
         */
        int value();
    private:
        mraa_aio_context m_aio;
};

/**
 * @brief C++ API for Grove Rotary Angle Sensor (Knob)
 *
 * Very basic UPM module for Grove rotary angle sensor (Knob) on analog. Provides
 * a set of functions to read the absolute pin value, degrees or radians and another
 * to do the same relative to the center of the knob's range.
 *
 * @ingroup grove analog
 * @snippet groverotary.cxx Interesting
 * @image html groverotary.jpeg
 */
class GroveRotary: public Grove {
    public:
        /**
         * Grove rotary angle sensor constructor
         *
         * @param analog pin to use
         */
        GroveRotary(unsigned int pin);
        /**
         * GroveRotary Destructor
         */
        ~GroveRotary();
        /**
         * Get absolute raw value from AIO pin
         *
         * @return the unsigned value from the ADC
         */
        float abs_value();
        /**
         * Get absolute raw degrees from AIO pin
         *
         * @return the unsigned degrees from the ADC
         */
        float abs_deg();
        /**
         * Get absolute raw radians from AIO pin
         *
         * @return the unsigned radians from the ADC
         */
        float abs_rad();
        /**
         * Get the relative value from the pin
         *
         * @return the signed value from the ADC
         */
        float rel_value();
        /**
         * Get relative degrees from AIO pin
         *
         * @return the signed degrees from the ADC
         */
        float rel_deg();
        /**
         * Get relative radians from AIO pin
         *
         * @return the signed radians from the ADC
         */
        float rel_rad();
    private:
        mraa_aio_context m_aio;
        static const int m_max_angle = 300;
};

/**
 * @brief C++ API for Grove slide potentiometer
 *
 * Very basic UPM module for Grove slide potentiometer on analog,
 * returns either raw value or scaled voltage value.
 *
 * @ingroup grove analog
 * @snippet groveslide.cxx Interesting
 * @image html groveslide.jpeg
 */
class GroveSlide: public Grove {
    public:
        /**
         * Grove analog slide potentiometer constructor
         *
         * @param analog pin to use
         */
        GroveSlide(unsigned int pin);
        /**
         * GroveSlide Destructor
         */
        ~GroveSlide();
        /**
         * Get raw value from AIO pin
         *
         * @return the raw value from the ADC
         */
        float raw_value();
        /**
         * Get the voltage value from the pin
         *
         * @param the reference voltage the board is set to as float (e.g. 3.3 or 5.0)
         *
         * @return the voltage reading based on the reference voltage passed as param
         */
        float voltage_value(float ref_vol);
    private:
        mraa_aio_context m_aio;
};

}
