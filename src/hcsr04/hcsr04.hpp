/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Author: Rafael Neri <rafael.neri@gmail.com>
 * Copyright (c) 2014-2015 Intel Corporation.
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

#include "hcsr04.h"

namespace upm {
/**
 * @brief HC-SR04 Ultrasonic Sensor
 * @defgroup hcsr04 libupm-hcsr04
 * @ingroup generic gpio sound
 */

/**
 * @library hcsr04
 * @sensor hcsr04
 * @comname Ultrasonic Distance Measuring Sensor
 * @type sound
 * @man generic
 * @con gpio
 * @web https://www.sparkfun.com/products/13959
 *
 * @brief API for the HC-SR04 Ultrasonic Sensor
 *
 * This module defines the HC-SR04 interface for libhcsr04
 *
 * @image html groveultrasonic.jpg
 * @snippet hcsr04.cxx Interesting
 */
class HCSR04 {
    public:
        /**
         * Instantiates an HCSR04 object
         *
         * @param triggerPin Pin to trigger the sensor for distance
         * @param echoPin Pulse response to triggering
         */
        HCSR04 (int triggerPin, int echoPin);
        /**
         * HCSR04 object destructor
         */
        ~HCSR04 ();

        /**
         * Gets the distance from the sensor
         *
         * @param unit Selects units for measurement
         */
        double getDistance (HCSR04_U unit);

    private:
        hcsr04_context m_hcsr04;
        HCSR04(const HCSR04& src) { /* do not create copied constructor */ }
        HCSR04& operator=(const HCSR04&) {return *this;}
    };
}
