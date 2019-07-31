/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * based on original C++ driver by
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <string>
#include <vector>
#include <mraa/pwm.hpp>
#include <mraa/initio.hpp>
#include <buzzer.h>

namespace upm {

    /**
     * @brief Piezoelectric Buzzers Library
     * @defgroup buzzer libupm-buzzer
     * @ingroup seeed pwm sound gsk
     */

    /**
     * @library buzzer
     * @sensor buzzer
     * @comname Buzzer
     * @type sound
     * @man seeed
     * @con pwm
     * @kit gsk
     *
     * @brief API for the Buzzer component
     *
     * This module defines the Buzzer interface for libbuzzer.
     * This sensor can make different tones when connected to
     * a pin capable of analog pulse-width modulation. It emits
     * sound using a piezoelectric material that vibrates at different
     * frequencies based on the input voltage.
     *
     * @image html buzzer.jpg
     * @snippet buzzer.cxx Interesting
     */

    class Buzzer {
    public:
        /**
         * Instantiates a Buzzer object.
         *
         * @param pinNumber Buzzer pin number
         */
        Buzzer(int pinNumber);

        /**
         * Instantiates a Buzzer object based on a given string.
         *
         * @param initStr string containing specific information for Buzzer initialization.
         */
        Buzzer(std::string initStr);

        /**
         * Buzzer object destructor.
         */
        ~Buzzer();

        /**
         * Plays a tone for a certain amount of time or indefinitely. When delay
         * is not used, the sound can be stopped by calling stopSound().
         *
         * @param note Note to play (DO, RE, MI, etc.) or frequency
         * @param delay Time in microseconds for which to play the
         * sound; if the value is 0, the sound is played indefinitely
         *
         * @return Note played
         */
        int playSound(int note, int delay);

        /**
         * Stops the sound currently playing. Should be called when playSound()
         * does not have a delay value.
         */
        void stopSound();

        /**
         * Sets the volume for the buzzer, but may affect the sound timbre.
         * Works best with halved values; e.g., 1.0, 0.5, 0.25, etc.
         *
         * @param vol Value to set the volume to, from 0.0 to 1.0
         */
        void setVolume(float vol);

        /**
         * Gets the buzzer volume.
         *
         * @return Value the volume was set to
         */
        float getVolume();

        /**
         * Returns the name of the sensor.
         *
         * @return Name of the sensor
         */
        std::string name()
        {
            return m_name;
        }

    protected:
        std::string m_name;
        mraa::MraaIo mraaIo;
        buzzer_context m_buzzer;
    private:
        /* Disable implicit copy and assignment operators */
        Buzzer(const Buzzer&) = delete;
        Buzzer &operator=(const Buzzer&) = delete;
    };
}
