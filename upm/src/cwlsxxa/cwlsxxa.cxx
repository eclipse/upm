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

#include "cwlsxxa.hpp"

using namespace upm;
using namespace std;

// for current loop reads that seems a little noisy, we average over
// several aio reads.
static const int maxSamples = 50;

// conversion from celsius to fahrenheit

static float c2f(float c)
{
  return (c * (9.0 / 5.0) + 32.0);
}


CWLSXXA::CWLSXXA(int gPin, int hPin, int tPin, float rResistor, float aref) :
  m_aioCO2(gPin), m_aioHum(0), m_aioTemp(0)
{
  m_hasHum = (hPin >= 0) ? true : false;
  m_hasTemp = (tPin >= 0) ? true : false;

  if (m_hasTemp)
    {
      m_aioTemp = new mraa::Aio(tPin);
      m_aResTemp = (1 << m_aioTemp->getBit());
    }
  else
    m_aResTemp = 0;

  if (m_hasHum)
    {
      m_aioHum = new mraa::Aio(hPin);
      m_aResHum = (1 << m_aioHum->getBit());
    }
  else
    m_aResHum = 0;

  m_aResCO2 = (1 << m_aioCO2.getBit());

  m_temperature = 0.0;
  m_humidity = 0.0;
  m_co2 = 0.0;

  m_aref = aref;
  m_rResistor = rResistor;
}

CWLSXXA::~CWLSXXA()
{
  if (m_aioHum)
    delete m_aioHum;
  if (m_aioTemp)
    delete m_aioTemp;
}

void CWLSXXA::update()
{
  // temperature
  int val;
  float volts;
  float milliamps = 0.0;

  int samples;
  if (m_rResistor)
    samples = maxSamples;
  else
    samples = 1;


  // temperature
  if (m_hasTemp)
    {
      // fortunately, this sensor always reports temperatures in C,
      // regardless of the configuration of the LCD display (for
      // models that have an LCD display)

      val = average(m_aioTemp, samples);

      volts = (float(val) * (m_aref / m_aResTemp));

      // valid range is 10-35C, current loop range is 16ma (20ma - 4ma)
      if (!m_rResistor)
        m_temperature = ((volts / m_aref) * 25.0) + 10.0;
      else
        {
          milliamps = ((volts / m_rResistor * 1000.0) - 4.0);
          if (milliamps < 0.0) // not connected
            milliamps = 0.0;
          m_temperature = (milliamps * (25.0 / 16.0)) + 10.0;
        }
    }

  // humidity
  if (m_hasHum)
    {
      val = average(m_aioHum, samples);
      volts = (float(val) * (m_aref / m_aResHum));

      // range is 0-100
      if (!m_rResistor)
        m_humidity = ((volts / m_aref) * 100.0);
      else
        {
          milliamps = ((volts / m_rResistor * 1000.0) - 4.0);
          if (milliamps < 0.0) // not connected
            milliamps = 0.0;
          m_humidity = milliamps * (100.0 / 16.0);
        }
    }

  // CO2
  val = average(&m_aioCO2, samples);
  volts = (float(val) * (m_aref / m_aResCO2));
  
  // CO2 range is 0-2000ppm
  if (!m_rResistor)
    m_co2 = ((volts / m_aref) * 2000.0);
  else
    {
      milliamps = ((volts / m_rResistor * 1000.0) - 4.0);
      if (milliamps < 0.0) // not connected
        milliamps = 0.0;
      m_co2 = milliamps * (2000.0 / 16.0);
    }
}

float CWLSXXA::getTemperature(bool fahrenheit)
{
  if (fahrenheit)
    return c2f(m_temperature);
  else
    return m_temperature;
}

float CWLSXXA::getHumidity()
{
  return m_humidity;
}

float CWLSXXA::getCO2()
{
  return m_co2;
}

int CWLSXXA::average(mraa::Aio *aio, int samples)
{
  if (samples <= 0)
    samples = 1;

  int avg = 0;
  for (int i=0; i<samples; i++)
    avg += aio->read();

  return (avg / samples);
}
