/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
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

#include "hdxxvxta.hpp"

using namespace upm;
using namespace std;

// conversion from celsius to fahrenheit

static float c2f(float c)
{
  return (c * (9.0 / 5.0) + 32.0);
}


HDXXVXTA::HDXXVXTA(int hPin, int tPin, float aref) :
  m_aioHum(hPin), m_aioTemp(0)
{
  if (tPin >= 0)
    m_hasTemp = true;
  else
    m_hasTemp = false;

  m_temperature = 0.0;
  m_humidity = 0.0;

  if (m_hasTemp)
    {
      m_aioTemp = new mraa::Aio(tPin);
      m_aResTemp = (1 << m_aioTemp->getBit());
    }
  else
    m_aResTemp = 0;

  m_aResHum = (1 << m_aioHum.getBit());
  m_aref = aref;

  // set the default temperature range to the A1 series (-40C-50C),
  // regardless of whether temperature measuring is enabled
  setRange(RANGE_MINUS40_50);
}

HDXXVXTA::~HDXXVXTA()
{
  if (m_aioTemp)
    delete m_aioTemp;
}

void HDXXVXTA::update()
{
  // temperature
  int val;
  float volts;

  if (m_hasTemp)
    {
      val = m_aioTemp->read();
      volts = (float(val) * (m_aref / m_aResTemp));

      switch (m_range)
        {
        case RANGE_MINUS40_50:
          // valid range is 50 + abs(-40) = 90
          m_temperature = ((volts / m_aref) * 90.0) - 40.0;
          break;
          
        case RANGE_0_50:
          // valid range is 50
          m_temperature = ((volts / m_aref) * 50.0);
          break;
          
        default:
          // shouldn't happen, but...
          throw std::out_of_range(std::string(__FUNCTION__) +
                                  ": internal error, invalid range value");
          break;
        }
    }

  // humidity
  val = m_aioHum.read();
  volts = (float(val) * (m_aref / m_aResHum));
  m_humidity = ((volts / m_aref) * 100.0);
}

float HDXXVXTA::getTemperature(bool fahrenheit)
{
  if (fahrenheit)
    return c2f(m_temperature);
  else
    return m_temperature;
}

float HDXXVXTA::getHumidity()
{
  return m_humidity;
}

