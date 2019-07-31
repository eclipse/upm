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

