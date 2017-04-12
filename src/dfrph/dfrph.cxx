/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
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

#include "dfrph.hpp"

using namespace upm;

DFRPH::DFRPH(int pin, float vref) : _dev(dfrph_init(pin))
{
    if (_dev == NULL)
        throw std::invalid_argument(std::string(__FUNCTION__) +
                ": dfrph_init() failed, invalid pin?");
}

DFRPH::~DFRPH()
{
    dfrph_close(_dev);
}

void DFRPH::setOffset(float offset)
{
    dfrph_set_offset(_dev, offset);
}

void DFRPH::setScale(float scale)
{
    dfrph_set_scale(_dev, scale);
}

float DFRPH::volts()
{
    float volts = 0.0;
    dfrph_get_raw_volts(_dev, &volts);
    return volts;
}

float DFRPH::pH(unsigned int samples)
{
    float ph_avg = 0.0;

    // Read at least 1 sample
    if (samples == 0) samples = 1;

    float ph = 0.0;
    for (unsigned int i = 0; i < samples; i++)
    {
        dfrph_get_ph(_dev, &ph);
        ph_avg += ph;
    }

    return ph_avg/samples;
}
