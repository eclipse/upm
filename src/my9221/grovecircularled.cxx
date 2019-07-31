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
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
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
