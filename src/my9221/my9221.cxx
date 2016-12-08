/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * These modules were rewritten, based on original work by:
 *
 * (original my9221/groveledbar driver)
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * (grovecircularled driver)
 * Author: Jun Kato and Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Contributions: Jon Trulson <jtrulson@ics.com>
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
#include <unistd.h>
#include <stdlib.h>

#include "my9221.hpp"

using namespace upm;
using namespace std;

MY9221::MY9221 (int dataPin, int clockPin, int instances) :
    m_my9221(my9221_init(dataPin, clockPin, instances))
{
    if (!m_my9221)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": my9221_init() failed");

}

MY9221::~MY9221()
{
    my9221_close(m_my9221);
}

void MY9221::setLED(int led, bool on)
{
    my9221_set_led(m_my9221, led, on);
}

void MY9221::setLowIntensityValue(int intensity)
{
    my9221_set_low_intensity_value(m_my9221, intensity);
}

void MY9221::setHighIntensityValue(int intensity)
{
    my9221_set_high_intensity_value(m_my9221, intensity);
}

void MY9221::setAll()
{
    my9221_set_all(m_my9221);
}

void MY9221::clearAll()
{
    my9221_clear_all(m_my9221);
}

void MY9221::refresh()
{
    my9221_refresh(m_my9221);
}

