/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * Based on original C++ driver by:
 * Author: Zion Orent <sorent@ics.com>
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

#include "speaker.h"

namespace upm {

    /**
     * @brief Speaker Library
     * @defgroup speaker libupm-speaker
     * @ingroup seeed gpio sound hak
     */

    /**
     * @library speaker
     * @sensor speaker libupm-speaker
     * @comname Speaker
     * @altname Grove Speaker
     * @type sound
     * @man seeed
     * @con gpio
     * @kit hak
     *
     * @brief API for the Speaker
     *
     * UPM module for the Speaker.
     * This sensor can generate different tones and sounds depending on the
     * frequency of the input signal.
     *
     * It can operate in one of two modes: GPIO (default) and PWM.
     *
     * Depending on which mode is selected, some methods may not be
     * usable.  In GPIO mode, the playAll() and playSound() methods
     * are supported.  In PWM mode, setFrequency(), emit(), on() and
     * off() are supported.  Calling a method not appropriate for the
     * mode will have no effect.
     *
     * @image html speaker.jpg
     * @snippet speaker.cxx Interesting
     * @snippet speaker-pwm.cxx Interesting
     */
    class Speaker {
    public:
        /**
         * Speaker constructor
         *
         * @param pin Digital pin to use
         * @param usePWM If true, PWM mode will be used, otherwise
         * GPIO mode (default) is used.
         */
        Speaker(int pin, bool usePWM=false);

        /**
         * Speaker destructor
         */

        virtual ~Speaker();
        /**
         * Plays all alto notes (lowest notes)
         *
         */

        void playAll();
        /**
         * Plays a sound and a note whether it's sharp or not
         *
         * @param letter Character name of the note
         * ('a', 'b', 'c', 'd', 'e', 'f', or 'g')
         * @param sharp If true, plays a sharp version of the note;
         * otherwise, does not play the note
         * @param vocalWeight String to determine whether to play a
         * low ("low"), a medium ("med"), or a high ("high") note
         */
        void playSound(char letter, bool sharp, std::string vocalWeight);

        /**
         * Emit a specific frequency for a given period of time and
         * return.  This function only operates when in PWM mode.  The
         * frequency is limited to between 50-32000Hz.  In addition,
         * the allowable frequencies may be restricted further by the
         * capabilities of your PWM hardware.
         *
         * @param freq The frequency to emit. Must be between 50 and 32000Hz
         * inclusive.
         * @param emit_ms The number of milliseconds to emit the frequency.
         */
        void emit(unsigned int freq, unsigned int emit_ms);

        /**
         * Set a default frequency to be used with on() and off().
         * This function only operates when in PWM mode.
         * The frequency is limited to between 50-32000Hz.  In
         * addition, the allowable frequencies may be restricted
         * further by the capabilities of your PWM hardware.
         *
         * @param freq The frequency to emit. Must be between 50 and 32000Hz
         * inclusive.
         */
        void setFrequency(unsigned int freq);

        /**
         * Turn the speaker on, and emit the frequency last specified
         * with setFrequency() or emit().  This function only operates
         * when in PWM mode.
         *
         */
        void on();

        /**
         * Turn the speaker off.  This function only operates when in
         * PWM mode.
         *
         */
        void off();

    protected:
        speaker_context m_speaker;

    private:
        /* Disable implicit copy and assignment operators */
        Speaker(const Speaker&) = delete;
        Speaker &operator=(const Speaker&) = delete;
    };
}
