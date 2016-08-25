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

GroveCircularLED::GroveCircularLED (uint8_t dataPin, uint8_t clockPin)
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

  for (uint8_t i=0; i<(LEDS_PER_INSTANCE * m_instances); i++)
    m_bitStates[i] = (i == position) ? m_highIntensity : m_lowIntensity;

  if (m_autoRefresh)
    refresh();

  return;
}

void GroveCircularLED::setLevel(uint8_t level, bool direction)
{
  if (level > 23)
    level = 23;

  if (!direction)
    {
      for (int i=0; i<(LEDS_PER_INSTANCE * m_instances); i++)
        m_bitStates[i] = (i < level) ? m_highIntensity : m_lowIntensity;
    }
  else
    {
      for (int i=0; i<(LEDS_PER_INSTANCE * m_instances); i++)
        m_bitStates[i] = (((LEDS_PER_INSTANCE * m_instances) - i) <= level)
          ? m_highIntensity : m_lowIntensity;
    }

  if (m_autoRefresh)
    refresh();

  return;
}
