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

#include "mraa/initio.hpp"
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

Buzzer::Buzzer(std::string initStr)
{
    mraa::MraaIo mraaIo(initStr);
    std::vector<std::string> upmTokens;

    if (mraaIo.getLeftoverStr() != "") {
      upmTokens = UpmStringParser::parse(mraaIo.getLeftoverStr());
    }

    m_buzzer = nullptr;
    if (!mraaIo.pwms.empty()) {
      m_buzzer = mraaIo.pwms[0];
    }

    if (m_buzzer == nullptr) {
      throw std::runtime_error(std::string(__FUNCTION__) +
            ": null buzzer context");
    }

    volume = 1.0;
    m_buzzer.enable(true);

    /*
    for (std::string tok : upmTokens) {
      if (tok.substr(0, 4) == "vol:") {
        // setVolume(::atof(tok.substr(4));
      } else {}
    }*/
}

Buzzer::~Buzzer()
{
    stopSound();
    m_buzzer.enable(false);
}

void Buzzer::setVolume(float vol)
{
    volume = vol;
}

float Buzzer::getVolume()
{
    return volume;
}

int Buzzer::playSound(int note, int delay)
{
    if (m_buzzer.period_us(note) != MRAA_SUCCESS) {
        cout << "period() error\n";
    }

    if (m_buzzer.write(volume * 0.5)) {
        cout << "write() error\n";
    }

    if (delay >= 0) {
        upm_delay_us(delay);
        stopSound();
    }

    return note;
}

void Buzzer::stopSound()
{
    m_buzzer.period_us(1);
    m_buzzer.write(0);
}

