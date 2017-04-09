/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015-2016 Intel Corporation.
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
#include <stdexcept>
#include "lm35.hpp"

using namespace std;
using namespace upm;

LM35::LM35(int pin, float aref) :
    m_lm35(lm35_init(pin, aref))
{
    if (!m_lm35)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lm35_init() failed");
}

LM35::~LM35()
{
    lm35_close(m_lm35);
}

float LM35::getTemperature()
{
    float temp;
    upm_result_t rv = lm35_get_temperature(m_lm35, &temp);

    if (rv)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lm35_get_temperature() failed");

  return temp;
}

void LM35::setScale(float scale)
{
    lm35_set_scale(m_lm35, scale);
}

void LM35::setOffset(float offset)
{
    lm35_set_offset(m_lm35, offset);
}
