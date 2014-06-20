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
#include <maa/pwm.h>

#define  DO     3300    // 261 Hz 3830
#define  RE     2930    // 294 Hz
#define  MI     2600    // 329 Hz
#define  FA     2460    // 349 Hz
#define  SOL    2190    // 392 Hz
#define  LA     1960    // 440 Hz
#define  SI     1750    // 493 Hz

namespace upm {

/**
 * @brief C++ API for Buzzer component
 *
 * This file defines the Buzzer C++ interface for libbuzzer
 *
 * @snippet buzzer-sound.cxx Interesting
 *
 */
class Buzzer {
    public:
        /**
         * Instanciates a Buzzer object
         *
         * @param pin Buzzer pin number
         */
        Buzzer (int pinNumber);

        /**
         * Buzzer object destructor.
         */
        ~Buzzer ();

        /**
         * Play chords.
         *
         * @param note chords (DO, RE, ME, etc...)
         * @param delay time in microsec for playing the sound
         */
        int playSound (int note, int delay);

        /**
         * Return name of the component
         */
        std::string name()
        {
            return m_name;
        }
    protected:
        std::string m_name;
    private:
        maa_pwm_context m_pwm_context;
};
}
