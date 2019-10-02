/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * based on original C++ driver by
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
#include <stdexcept>
#include <unistd.h>
#include <string>
#include <iostream>

#include "upm_string_parser.hpp"
#include <upm_utilities.h>
#include "buzzer.hpp"


using namespace upm;
using namespace std;

Buzzer::Buzzer(int pinNumber) : m_buzzer(buzzer_init(pinNumber))
{
    if (!m_buzzer)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": buzzer_init() failed");
}

Buzzer::Buzzer(std::string initStr) : mraaIo(initStr)
{
    mraa_io_descriptor* descs = mraaIo.getMraaDescriptors();
    std::vector<std::string> upmTokens;

    if (mraaIo.getLeftoverStr() != "") {
        upmTokens = UpmStringParser::parse(mraaIo.getLeftoverStr());
    }

    m_buzzer = (buzzer_context)malloc(sizeof(struct _buzzer_context));

    if (!m_buzzer)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": buzzer_init() failed");

    m_buzzer->pwm = NULL;
    m_buzzer->volume = 0.0;
    m_buzzer->initialized = false;

    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        buzzer_close(m_buzzer);
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": mraa_init() failed");
    }

    if(!descs->pwms)
    {
        buzzer_close(m_buzzer);
        throw std::runtime_error(std::string(__FUNCTION__) +
                                ": mraa_pwm_init() failed");
    }
    else
    {
        if( !(m_buzzer->pwm = descs->pwms[0]) )
        {
            buzzer_close(m_buzzer);
            throw std::runtime_error(std::string(__FUNCTION__) +
                                ": mraa_pwm_init() failed");
        }
    }
    mraa_pwm_enable(m_buzzer->pwm, 1);

    for (std::string tok : upmTokens) {
        if(tok.substr(0, 4) == "vol:") {
            float vol = std::stof(tok.substr(4));
            setVolume(vol);
        }
        if(tok.substr(0, 5) == "play:") {
            std::string::size_type sz;
            int note  = std::stoi(tok.substr(5), &sz);
            tok = tok.substr(5);
            int delay = std::stoi(tok.substr(sz+1));
            playSound(note, delay);
        }
    }

    m_buzzer->initialized = true;
}

Buzzer::~Buzzer()
{
    buzzer_close(m_buzzer);
}

void Buzzer::setVolume(float vol)
{
    buzzer_set_volume(m_buzzer, vol);
}

float Buzzer::getVolume()
{
    return buzzer_get_volume(m_buzzer);
}

int Buzzer::playSound(int note, int delay)
{
    if (buzzer_play_sound(m_buzzer, note, delay))
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": buzzer_play_sound() failed");
    return note;
}

void Buzzer::stopSound()
{
    if (buzzer_stop_sound(m_buzzer))
        throw std::runtime_error(std::string(__FUNCTION__) +
                                ": buzzer_stop_sound() failed");
}

