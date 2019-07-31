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

#include "groveledbar.hpp"

using namespace upm;
using namespace std;

GroveLEDBar::GroveLEDBar (int dataPin, int clockPin, int instances)
    : MY9221(dataPin, clockPin, instances)
{
    // auto refresh by default
    setAutoRefresh(true);
    clearAll();
}

GroveLEDBar::~GroveLEDBar()
{
}

void GroveLEDBar::setBarLevel(uint8_t level, bool greenToRed,
                              unsigned int barNumber)
{
    // here we manipulate the my9221 context struct directly
    if (level > 10)
        level = 10;

    if (barNumber >= m_my9221->instances)
        barNumber = m_my9221->instances - 1;

    unsigned int ledsPerInstance = m_my9221->max_leds_per_instance;
    unsigned int start = barNumber * ledsPerInstance;
    unsigned int end = start + ledsPerInstance;

    if (!greenToRed)
    {
        for (unsigned int i=start; i<end; i++)
            m_my9221->bitStates[i] = (i < (level + start)) ?
                m_my9221->highIntensity : m_my9221->lowIntensity;
    }
    else
    {
        for (unsigned int i=start; i<end; i++)
            m_my9221->bitStates[i] = ( ((start + ledsPerInstance) - i) <=
                               (level + 2 + start)) ?
                m_my9221->highIntensity : m_my9221->lowIntensity;
    }

    if (m_my9221->autoRefresh)
        refresh();

    return;
}
