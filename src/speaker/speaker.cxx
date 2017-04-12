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

#include <iostream>
#include <string>
#include <stdexcept>

#include "speaker.hpp"

using namespace upm;

Speaker::Speaker(int pin, bool usePWM) :
    m_speaker(nullptr)
{
    if (usePWM)
        m_speaker = speaker_init_pwm(pin);
    else
        m_speaker = speaker_init(pin);

    if (!m_speaker)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": speaker_init()/speaker_init_pwm() failed.");
}

Speaker::~Speaker()
{
    speaker_close(m_speaker);
}

void Speaker::playAll()
{
    speaker_play_all(m_speaker);
}

void Speaker::playSound(char letter, bool sharp, std::string vocalWeight)
{
    speaker_play_sound(m_speaker, letter, sharp, vocalWeight.c_str());
}

void Speaker::emit(unsigned int freq, unsigned int emit_ms)
{
    if (speaker_emit(m_speaker, freq, emit_ms))
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": speaker_emit() failed.");
}

void Speaker::setFrequency(unsigned int freq)
{
    if (speaker_set_frequency(m_speaker, freq))
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": speaker_set_frequency() failed.");
}

void Speaker::on()
{
    speaker_on(m_speaker);
}

void Speaker::off()
{
    speaker_off(m_speaker);
}
