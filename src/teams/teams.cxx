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

#include "teams.hpp"

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


TEAMS::TEAMS(int tPin, float rResistor, float aref) :
  m_aioTemp(tPin)
{
  if (rResistor < 0.0)
    {
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": rResistor must be >= 0.0");
    }

  m_aResTemp = (1 << m_aioTemp.getBit());

  m_temperature = 0.0;

  m_aref = aref;
  m_rResistor = rResistor;
  m_connected = false;

  // this will only be non-zero when using a direct 4-20ma interface
  // like libelium
  m_rawMilliamps = 0.0;

  m_offset = 0.0;
}

TEAMS::~TEAMS()
{
}

void TEAMS::update()
{
  float milliamps = 0.0;

  int val = average(maxSamples);
  float volts = (float(val) * (m_aref / m_aResTemp));

  // valid temp range is 25.0 (35C - 10C), current loop range is 16ma
  // (20ma - 4ma)
  if (m_rResistor)
    {
      // direct 4-20 current loop interface
      milliamps = (volts / m_rResistor * 1000.0) + m_offset;
      m_rawMilliamps = milliamps;

      // subtract 0 (4ma) value
      milliamps -= 4.0;
      if (milliamps < 0.0) // not connected
        {
          milliamps = 0.0;
          m_connected = false;
        }
      else
        m_connected = true;

      m_temperature = (milliamps * (25.0 / 16.0)) + 10.0;
    }
  else
    {
      // normal analog read, already scaled to 0-5v, always connected
      m_temperature = ((volts / m_aref) * 25.0) + 10.0;
      m_connected = true;
    }
}

float TEAMS::getTemperature(bool fahrenheit)
{
  if (fahrenheit)
    return c2f(m_temperature);
  else
    return m_temperature;
}

float TEAMS::getTemperature()
{
  return getTemperature(false);
}

int TEAMS::average(int samples)
{
  if (samples <= 0)
    samples = 1;

  int avg = 0;
  for (int i=0; i<samples; i++)
    avg += m_aioTemp.read();

  return (avg / samples);
}
