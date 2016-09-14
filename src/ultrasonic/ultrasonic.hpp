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

namespace upm {

/**
 * @brief Grove ultrasonic sensor library
 * @defgroup ultrasonic libupm-ultrasonic
 * @ingroup seeed gpio sound
 */

/**
 * @library ultrasonic
 * @sensor ultrasonic
 * @comname Grove Ultrasonic Ranger
 * @type sound
 * @man seeed
 * @con gpio
 *
 * @brief API for Grove Ultrasonic Ranger
 *
 * This Grove Ultrasonic sensor is a non-contact distance measurement module
 * which is compatible with the Grove system. It is designed for easy modular
 * project usage with industrial performance. Detection ranges from 3 cm (1.2")
 * to 4 m (13'1.5") and works best when the object is within a 30 degree angle
 * relative to the sensor.
 *
 * @image html ultrasonic.jpg
 * @snippet ultrasonic.cxx Interesting
 */
class UltraSonic {
    public:
        /**
         * Instantiates a UltraSonic object
         *
         * @param pin pin for triggering the sensor for distance and for receiving pulse response
         */
        UltraSonic (uint8_t pin);

        /**
         * UltraSonic object destructor.
         */
        ~UltraSonic ();

        /**
         * Returns the echo's pulse width from the sensor in microseconds.
         * Divide by 58 to convert distance to centimetres. 
         * Divide by 148 to convert distance to inches.
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
         * Returns true while the sensor is busy waiting for the echo pulse
         */
        bool working()
        {
            return m_doWork;
        }

    private:
        bool m_doWork; /* Flag to control blocking function while waiting for falling edge interrupt */
        mraa_gpio_context m_pinCtx;
        uint8_t m_InterruptCounter;
        struct timeval m_RisingTimeStamp;
        struct timeval m_FallingTimeStamp;
        std::string m_name;

        /**
         * ISR for the pulse signal
         */
        static void signalISR(void *ctx);

        /**
         * On each interrupt this function will detect if the interrupt
         * was falling edge or rising.
         * Should be called from the interrupt handler.
         */
        void ackEdgeDetected ();
};

}
