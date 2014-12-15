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
   * @brief C++ API for the GroveSpeaker speaker
   *
   * UPM module for the GroveSpeaker.
   * This sensor can generate different tones and sounds depending on the
   * frequency of the input signal.
   *
   * @ingroup gpio
   * @snippet grovespeaker.cxx Interesting
   */
  class GroveSpeaker {
  public:
    /**
     * GroveSpeaker Constructor
     *
     * @param pin digital pin to use
     */
    GroveSpeaker(int pin);
    /**
     * GroveSpeaker Destructor
     */
    ~GroveSpeaker();
    /**
     * Play all alto notes (lowest notes)
     *
     */
	void playAll();
    /**
     * Play a sound and note whether it's sharp or not
     *
     * @param letter character name of note 
     * ('a', 'b', 'c', 'd', 'e', 'f', or 'g')
     * @param sharp if true, play sharp version of note; otherwise, do not
     * @param vocalWeight string to determine whether to play low ("low"), 
     * medium ("med"), or high ("high") note
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


