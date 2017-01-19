/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * Based on original C++ driver by:
 * Author: Zion Orent <sorent@ics.com>
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

#include "speaker.h"

namespace upm {

    /**
     * @brief Speaker library
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
     * @image html speaker.jpg
     * @snippet speaker.cxx Interesting
     */
    class Speaker {
    public:
        /**
         * Speaker constructor
         *
         * @param pin Digital pin to use
         */
        Speaker(int pin);

        /**
         * Speaker destructor
         */
        ~Speaker();
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

    protected:
        speaker_context m_speaker;

    private:
    };
}
