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

#include "grovecircularled.hpp"

using namespace upm;
using namespace std;

GroveCircularLED::GroveCircularLED (int dataPin, int clockPin)
    : MY9221(dataPin, clockPin, 2)
{
    // auto refresh by default
    setAutoRefresh(true);
    clearAll();
}

GroveCircularLED::~GroveCircularLED()
{
}

void GroveCircularLED::setSpinner(uint8_t position)
{
    if (position > 23)
        position = 23;

    unsigned int ledsPerInstance = m_my9221->max_leds_per_instance;

    for (uint8_t i=0; i<(ledsPerInstance * m_my9221->instances); i++)
        m_my9221->bitStates[i] =
            (i == position) ? m_my9221->highIntensity : m_my9221->lowIntensity;

    if (m_my9221->autoRefresh)
        refresh();

    return;
}

void GroveCircularLED::setLevel(uint8_t level, bool direction)
{
    if (level > 23)
        level = 23;

    unsigned int ledsPerInstance = m_my9221->max_leds_per_instance;

    if (!direction)
    {
        for (unsigned int i=0; i < (ledsPerInstance * m_my9221->instances); i++)
            m_my9221->bitStates[i] =
                (i < level) ? m_my9221->highIntensity : m_my9221->lowIntensity;
    }
    else
    {
        for (unsigned int i=0; i<(ledsPerInstance * m_my9221->instances); i++)
            m_my9221->bitStates[i] =
                (((ledsPerInstance * m_my9221->instances) - i) <= level)
                ? m_my9221->highIntensity : m_my9221->lowIntensity;
    }

    if (m_my9221->autoRefresh)
        refresh();

    return;
}
