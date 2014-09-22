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
 * @ingroup grove
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
 * @ingroup grove
 * @snippet grovetemp.cxx Interesting
 */
class GroveTemp: public Grove {
    public:
        GroveTemp(unsigned int pin);
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
 * @ingroup grove
 * @snippet grovelight.cxx Interesting
 */
class GroveLight: public Grove {
    public:
        GroveLight(unsigned int pin);
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

}
