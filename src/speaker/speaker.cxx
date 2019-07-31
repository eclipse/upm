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
