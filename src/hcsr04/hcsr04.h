 /*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
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
#include <mraa/pwm.h>
#include <sys/time.h>

#define HIGH                   1
#define LOW                    0

#define MAX_PERIOD             7968
#define TRIGGER_PULSE          10

namespace upm {

/**
 * @brief HCSR04 ultrasonic sensor library
 * @defgroup hcsr04 libupm-hcsr04
 */

/**
 * @brief C++ API for HCSR04 (ultrasonic ranging module) component
 *
 * This file defines the HCSR04 C++ interface for libhcsr04
 *
 * @ingroup hcsr04 gpio
 * @snippet hcsr04.cxx Interesting
 */
class HCSR04 {
    public:
        /**
         * Instanciates a HCSR04 object
         *
         * @param triggerPin pin for triggering the sensor for distance
         * @param echoPin pulse response to triggering
         * @param fptr function pointer for handling raising and
         * falling interrupts
         */
        HCSR04 (uint8_t triggerPin, uint8_t echoPin, void (*fptr)(void *));

        /**
         * HCSR04 object destructor.
         */
        ~HCSR04 ();

        /**
         * Get the distance from the sensor.
         */
        int getDistance ();

        /**
         * On each interrupt this function will detect if the interrupt
         * was falling edge or rising.
         * Should be called from the interrupt handler.
         */
        void ackEdgeDetected ();

        uint8_t m_doWork; /**< Flag to controll blocking function while waiting for falling edge interrupt */

        /**
         * Return name of the component
         */
        std::string name()
        {
            return m_name;
        }

    private:
        mraa_pwm_context     m_pwmTriggerCtx;
        mraa_gpio_context    m_echoPinCtx;

        uint8_t m_waitEcho;
        long    m_RisingTimeStamp;
        long    m_FallingTimeStamp;
        uint8_t m_InterruptCounter;

        std::string         m_name;
};

}
