/*
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
#include <map>
#include <unistd.h>
#include <mraa/gpio.h>

#define HIGH      1
#define LOW       0

namespace upm {
  /**
   * @brief Grove Speaker library
   * @defgroup grovespeaker libupm-grovespeaker
   * @ingroup seeed gpio sound hak
   */
typedef struct
{
    int delayTimeLow;
    int delayTimeLowSharp;
    int delayTimeMed;
    int delayTimeMedSharp;
    int delayTimeHigh;
    int delayTimeHighSharp;
} NoteData;
  /**
   * @library grovespeaker
   * @sensor grovespeaker
   * @comname Grove Speaker
   * @type sound
   * @man seeed
   * @con gpio
   * @kit hak
   *
   * @brief API for the Grove Speaker
   *
   * UPM module for the Grove Speaker.
   * This sensor can generate different tones and sounds depending on the
   * frequency of the input signal.
   * 
   * @image html grovespeaker.jpg 
   * @snippet grovespeaker.cxx Interesting
   */
  class GroveSpeaker {
  public:
    /**
     * Grove Speaker constructor
     *
     * @param pin Digital pin to use
     */
    GroveSpeaker(int pin);
    /**
     * GroveSpeaker destructor
     */
    ~GroveSpeaker();
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
     * @param sharp If true, plays a sharp version of the note; otherwise, does not play the note
     * @param vocalWeight String to determine whether to play a low ("low"),
     * a medium ("med"), or a high ("high") note
     */
    void playSound(char letter, bool sharp, std::string vocalWeight);

  private:
        mraa_gpio_context m_gpio;
        std::map <char, NoteData> m_note_list;
        void sound(int note_delay);
        NoteData storeNote(int noteDelayLow, int noteDelayLowSharp,
                           int noteDelayMed, int noteDelayMedSharp,
                           int noteDelayHigh, int noteDelayHighSharp);
  };
}
