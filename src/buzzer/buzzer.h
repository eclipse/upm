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
#include <mraa/pwm.h>

#define  DO     3300    // 261 Hz 3830
#define  RE     2930    // 294 Hz
#define  MI     2600    // 329 Hz
#define  FA     2460    // 349 Hz
#define  SOL    2190    // 392 Hz
#define  LA     1960    // 440 Hz
#define  SI     1750    // 493 Hz

namespace upm {

/**
 * @brief Buzzer library
 * @defgroup buzzer libupm-buzzer
 * @ingroup seeed pwm sound gsk
 */

/**
 * @library buzzer
 * @sensor buzzer
 * @comname Grove Buzzer
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
 * @snippet buzzer-sound.cxx Interesting
 */
class Buzzer {
    public:
        /**
         * Instantiates a Buzzer object.
         *
         * @param pinNumber Buzzer pin number
         */
        Buzzer (int pinNumber);

        /**
         * Buzzer object destructor.
         */
        ~Buzzer ();

        /**
         * Plays a tone for a certain amount of time or indefinitely. When delay
         * is not used, the sound can be stopped by calling stopSound().
         *
         * @param note Note to play (C, D, E, etc.) or frequency
         * @param delay Time in microseconds for which to play the sound; if the value is
         * 0, the sound is played indefinitely
         *
         * @return Note played
         */
        int playSound (int note, int delay);

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
    private:
        mraa_pwm_context m_pwm_context;
        float m_volume;
};
}
