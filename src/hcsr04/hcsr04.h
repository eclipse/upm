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

#include <string>
#include <mraa/aio.h>
#include <mraa/gpio.h>
#include <mraa/pwm.h>
#include <sys/time.h>

#if defined(SWIGJAVA) || defined(JAVACALLBACK)
#include "../IsrCallback.h"
#endif

#define CM 1
#define INC 0

namespace upm {
/**
 * @brief HC-SR04 Ultrasonic Sensor library
 * @defgroup hcsr04 libupm-hcsr04
 * @ingroup generic gpio sound
 */

/**
 * @library hcsr04
 * @sensor hcsr04
 * @comname HC-SR04 Ultrasonic Sensor
 * @type sound
 * @man generic
 * @con gpio
 *
 * @brief API for the HC-SR04 Ultrasonic Sensor
 *
 * This module defines the HC-SR04 interface for libhcsr04
 *
 * @snippet hcsr04.cxx Interesting
 */
class HCSR04 {
    public:
        /**
         * Instantiates an HCSR04 object
         *
         * @param triggerPin Pin to trigger the sensor for distance
         * @param echoPin Pulse response to triggering
         * @param fptr Function pointer to handle rising-edge and
         * falling-edge interrupts
         */
#if defined(SWIGJAVA) || defined(JAVACALLBACK)
        HCSR04 (uint8_t triggerPin, uint8_t echoPin, IsrCallback *cb);
#else
        HCSR04 (uint8_t triggerPin, uint8_t echoPin, void (*fptr)(void *));
#endif
        /**
         * HCSR04 object destructor
         */
        ~HCSR04 ();

        /**
         * Gets the distance from the sensor
         */
        double getDistance (int sys);

        /**
         * On each interrupt, this function detects if the interrupt
         * was falling-edge or rising-edge.
         * Should be called from the interrupt handler.
         */
        void ackEdgeDetected ();

        uint8_t m_doWork; /**< Flag to control blocking function while waiting for a falling-edge interrupt */

        /**
         * Returns the name of the sensor
         */
        std::string name()
        {
            return m_name;
        }

    private:
#if defined(SWIGJAVA) || defined(JAVACALLBACK)
        HCSR04 (uint8_t triggerPin, uint8_t echoPin, void (*fptr)(void *));
#endif
        double timing();
        mraa_gpio_context   m_triggerPinCtx;
        mraa_gpio_context   m_echoPinCtx;

        long    m_RisingTimeStamp;
        long    m_FallingTimeStamp;
        uint8_t m_InterruptCounter;

        std::string         m_name;
};

}
