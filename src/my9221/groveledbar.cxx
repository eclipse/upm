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

#include "groveledbar.hpp"

using namespace upm;
using namespace std;

GroveLEDBar::GroveLEDBar (uint8_t dataPin, uint8_t clockPin, int instances)
  : MY9221(dataPin, clockPin, instances)
{
  // auto refresh by default
  setAutoRefresh(true);
  clearAll();
}

GroveLEDBar::~GroveLEDBar()
{
}

void GroveLEDBar::setBarLevel(uint8_t level, bool greenToRed, int barNumber)
{
  if (level > 10)
    level = 10;

  if (barNumber >= m_instances)
    barNumber = m_instances - 1;

  int start = barNumber * LEDS_PER_INSTANCE;
  int end = start + LEDS_PER_INSTANCE;

  if (!greenToRed)
    {
      for (int i=start; i<end; i++)
        m_bitStates[i] = (i < (level + start)) ?
          m_highIntensity : m_lowIntensity;
    }
  else
    {
      for (int i=start; i<end; i++)
        m_bitStates[i] = ( ((start + LEDS_PER_INSTANCE) - i) <=
                           (level + 2 + start)) ?
                           m_highIntensity : m_lowIntensity;
    }

  if (m_autoRefresh)
    refresh();

  return;
}
