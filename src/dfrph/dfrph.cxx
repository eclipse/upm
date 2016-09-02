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

#include "dfrph.hpp"

using namespace upm;

DFRPH::DFRPH(int pin, float aref)
{
   _dev = dfrph_init(pin);
   addPin(pin, _dev);
}

float DFRPH::pH(unsigned int samples)
{
    if (!samples)
        samples = 1;

    float avg = 0.0;

    for (int i=0; i<samples; i++)
        avg += getSensorValue();

    avg /= samples;

    return avg;
}

float DFRPH::getSensorValue(int16_t pin) const
{
    std::map<int16_t, analog_sensor_t*>::const_iterator it =
        _pinmap.begin();

    /* Use default or throw if invalid pin */
    if (pin != -1)
        it = _pinmap.find(pin);

    if (it == _pinmap.end())
        throw std::invalid_argument(std::string(__FUNCTION__) +
                ": Invalid AIO pin");

    float value = 0.0;
    // Call C get_ph method
    if (dfrph_get_ph(_dev, &value) != UPM_SUCCESS)
        throw std::invalid_argument(std::string(__FUNCTION__) +
                ": dfrph_get_ph() failed, invalid pin?");
    return value;
}
