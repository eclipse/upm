/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Author: Rafael Neri <rafael.neri@gmail.com>
 * Copyright (c) 2014-2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include "hcsr04.h"
#include <interfaces/iDistance.hpp>

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
class HCSR04 : virtual public iDistance {
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

        /**
         * Gets the distance from the sensor
         *
         * @return distance measured in cm.
         */
        float getDistance();
    private:
        hcsr04_context m_hcsr04;
        HCSR04(const HCSR04& src) { /* do not create copied constructor */ }
        HCSR04& operator=(const HCSR04&) {return *this;}
    };
}
