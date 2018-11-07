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

#include <unistd.h>
#include <iostream>
#include <stdexcept>

#include "tzemt400.hpp"

using namespace upm;
using namespace std;

// conversion from fahrenheit to celsius and back

static float f2c(float f)
{
  return ((f - 32.0) / (9.0 / 5.0));
}

static float c2f(float c)
{
  return (c * (9.0 / 5.0) + 32.0);
}

TZEMT400::TZEMT400(int nodeID) :
  ozwInterface(nodeID)
{
}

TZEMT400::~TZEMT400()
{
}

float TZEMT400::getTemperature(bool fahrenheit)
{
  float temp = m_instance->getValueAsFloat(m_nodeID, INDEX_Temperature);
  uint8_t isFahrenheit = m_instance->getValueAsByte(m_nodeID, INDEX_FC_Units);

  if (fahrenheit)
    {
      if (isFahrenheit)
        return temp;
      else
        return c2f(temp);
    }
  else
    {
      if (isFahrenheit)
        return f2c(temp);
      else
        return temp;
    }
}

string TZEMT400::getMode()
{
  return m_instance->getValueAsString(m_nodeID, INDEX_Mode);
}

string TZEMT400::getOperatingState()
{
  return m_instance->getValueAsString(m_nodeID, INDEX_Operating_State);
}

float TZEMT400::getHeatingPointTemperature(bool fahrenheit)
{
  float temp = m_instance->getValueAsFloat(m_nodeID, INDEX_Heating_Point);
  uint8_t isFahrenheit = m_instance->getValueAsByte(m_nodeID, INDEX_FC_Units);

  if (fahrenheit)
    {
      if (isFahrenheit)
        return temp;
      else
        return c2f(temp);
    }
  else
    {
      if (isFahrenheit)
        return f2c(temp);
      else
        return temp;
    }
}

float TZEMT400::getCoolingPointTemperature(bool fahrenheit)
{
  float temp = m_instance->getValueAsFloat(m_nodeID, INDEX_Cooling_Point);
  uint8_t isFahrenheit = m_instance->getValueAsByte(m_nodeID, INDEX_FC_Units);

  if (fahrenheit)
    {
      if (isFahrenheit)
        return temp;
      else
        return c2f(temp);
    }
  else
    {
      if (isFahrenheit)
        return f2c(temp);
      else
        return temp;
    }
}

string TZEMT400::getFanMode()
{
  return m_instance->getValueAsString(m_nodeID, INDEX_Fan_Mode);
}

string TZEMT400::getFanState()
{
  return m_instance->getValueAsString(m_nodeID, INDEX_Fan_State);
}

void TZEMT400::update()
{
  m_instance->refreshValue(m_nodeID, INDEX_Temperature);
  m_instance->refreshValue(m_nodeID, INDEX_Mode);
  m_instance->refreshValue(m_nodeID, INDEX_Operating_State);
  m_instance->refreshValue(m_nodeID, INDEX_Heating_Point);
  m_instance->refreshValue(m_nodeID, INDEX_Cooling_Point);
  m_instance->refreshValue(m_nodeID, INDEX_Fan_Mode);
  m_instance->refreshValue(m_nodeID, INDEX_Fan_State);
  m_instance->refreshValue(m_nodeID, INDEX_FC_Units);
}
