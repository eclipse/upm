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

MY9221::MY9221 (uint8_t dataPin, uint8_t clockPin, int instances)
  : m_gpioData(dataPin), m_gpioClk(clockPin), m_bitStates(0)
{
  if (instances < 1)
    {
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": instances must be at least 1");
    }

  // set directions
  m_gpioClk.dir(mraa::DIR_OUT);
  m_gpioData.dir(mraa::DIR_OUT);
  
  // we warn if these fail, since it may not be possible to handle
  // more than one instance

  if (m_gpioClk.useMmap(true) != mraa::SUCCESS)
    cerr << __FUNCTION__
         << ": Warning: mmap of Clk pin failed, correct operation "
         << "may be affected."
         << endl;

  if (m_gpioData.useMmap(true) != mraa::SUCCESS)
    cerr << __FUNCTION__
         << ": Warning: mmap of Data pin failed, correct operation "
         << "may be affected."
         << endl;

  setLowIntensityValue(0x00);   // full off
  setHighIntensityValue(0xff);  // full brightness

  m_commandWord = 0x0000;       // all defaults
  m_instances = instances;

  m_bitStates = new uint16_t[instances * LEDS_PER_INSTANCE];

  setAutoRefresh(true);
  clearAll();
}

MY9221::~MY9221()
{
  clearAll();

  if (!m_autoRefresh)
    refresh();

  delete m_bitStates;
}

void MY9221::setLED(int led, bool on)
{
  int maxLed = (LEDS_PER_INSTANCE * m_instances) - 1;

  if (led > maxLed)
    led = maxLed;
  if (led < 0)
    led = 0;

  m_bitStates[led] = (on) ? m_highIntensity : m_lowIntensity;

  if (m_autoRefresh)
    refresh();
}

void MY9221::setLowIntensityValue(int intensity)
{
  m_lowIntensity = (intensity & 0xff);
}

void MY9221::setHighIntensityValue(int intensity)
{
  m_highIntensity = (intensity & 0xff);
}

void MY9221::setAll()
{
  for (int i=0; i<(m_instances * LEDS_PER_INSTANCE); i++)
    m_bitStates[i] = m_highIntensity;

  if (m_autoRefresh)
    refresh();
}

void MY9221::clearAll()
{
  for (int i=0; i<(m_instances * LEDS_PER_INSTANCE); i++)
    m_bitStates[i] = m_lowIntensity;

  if (m_autoRefresh)
    refresh();
}

void MY9221::refresh()
{
  for (int i=0; i<(m_instances * LEDS_PER_INSTANCE); i++)
    {
      if (i % 12 == 0)
        {
          send16bitBlock(m_commandWord);
        }
      send16bitBlock(m_bitStates[i]);
    }

  lockData();
}

void MY9221::lockData()
{
  m_gpioData.write(0);
  usleep(220);

  for(int idx = 0; idx < 4; idx++)
    {
      m_gpioData.write(1);
      m_gpioData.write(0);
    }

  // in reality, we only need > 200ns + (m_instances * 10ns), so the
  // following should be good for up to m_instances < 80), if the
  // datasheet is to be believed :)
  usleep(1);

  return;
}

void MY9221::send16bitBlock(uint16_t data)
{
  for (uint8_t bit_idx = 0; bit_idx < 16; bit_idx++)
    {
      uint32_t state = (data & 0x8000) ? 1 : 0;
      m_gpioData.write(state);
      state = m_gpioClk.read();

      if (state)
        state = 0;
      else
        state = 1;
      
      m_gpioClk.write(state);
      
      data <<= 1;
    }
  return;
}
