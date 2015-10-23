/*
 * Author: Jun Kato <i@junkato.jp>
 * Copyright (c) 2015 Jun Kato.
 *
 * Thanks to Seeed Studio for a working arduino sketch
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
#include <sys/time.h>

#define HIGH                   1
#define LOW                    0

#define MAX_PERIOD             7968
#define TRIGGER_PULSE          10

namespace upm {

/**
 * @brief Grove ultrasonic sensor library
 * @defgroup groveultrasonic libupm-groveultrasonic
 */

/**
 * @brief C++ API for Grove ultrasonic ranging component
 *
 * This file defines the GroveUltraSonic C++ interface for libgroveultrasonic
 *
 * @ingroup groveultrasonic gpio
 */
class GroveUltraSonic {
    public:
        /**
         * Instanciates a GroveUltraSonic object
         *
         * @param pin pin for triggering the sensor for distance and for receiving pulse response
         */
        GroveUltraSonic (uint8_t pin);

        /**
         * GroveUltraSonic object destructor.
         */
        ~GroveUltraSonic ();

        /**
         * Get the distance from the sensor.
         */
        int getDistance ();

        /**
         * Return name of the component
         */
        std::string name()
        {
            return m_name;
        }

        /**
         * ISR for the pulse signal
         */
        static void signalISR(void *ctx);

        /**
         * Flag to controll blocking function while waiting for falling edge interrupt
         */
        uint8_t m_doWork;

    private:
        mraa_gpio_context m_pinCtx;

        uint8_t m_InterruptCounter;
        struct timeval m_RisingTimeStamp;
        struct timeval m_FallingTimeStamp;

        std::string m_name;

        /**
         * On each interrupt this function will detect if the interrupt
         * was falling edge or rising.
         * Should be called from the interrupt handler.
         */
        void ackEdgeDetected ();
};

}
