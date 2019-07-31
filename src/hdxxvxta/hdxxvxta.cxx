/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
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

