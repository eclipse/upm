/*
 * Author: Jun Kato <i@junkato.jp>
 * Copyright (c) 2015 Jun Kato.
 *
 * Thanks to Seeed Studio for a working arduino sketch
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
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
 * @brief Ultrasonic Sensor
 * @defgroup ultrasonic libupm-ultrasonic
 * @ingroup seeed gpio sound
 */

/**
 * @library ultrasonic
 * @sensor ultrasonic
 * @comname Ultrasonic Proximity Sensor
 * @type sound
 * @man seeed
 * @web http://wiki.seeed.cc/Grove-Ultrasonic_Ranger/
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
        UltraSonic (int pin);

        /**
         * UltraSonic object destructor.
         */
        ~UltraSonic ();

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
        float working()
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
