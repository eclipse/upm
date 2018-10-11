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
 * @deprecated This library is being replaced by libupm-ultrasonic
 * @brief Grove Ultrasonic Sensor
 * @defgroup groveultrasonic libupm-groveultrasonic
 * @ingroup seeed gpio sound
 */

/**
 * @deprecated This class is being replaced by UltraSonic
 * @library groveultrasonic
 * @sensor groveultrasonic
 * @comname Ultrasonic Proximity Sensor
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
 * @image html groveultrasonic.jpg
 * @snippet groveultrasonic.cxx Interesting
 */
class GroveUltraSonic {
    public:
        /**
         * Instantiates a GroveUltraSonic object
         *
         * @param pin pin for triggering the sensor for distance and for receiving pulse response
         */
        GroveUltraSonic (int pin);

        /**
         * GroveUltraSonic object destructor.
         */
        ~GroveUltraSonic ();

        /**
         * Returns the echo's pulse width from the sensor in microseconds.
         * Divide by 58 to convert distance to centimetres. 
         * Divide by 148 to convert distance to inches.
         */
        float getDistance ();

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
