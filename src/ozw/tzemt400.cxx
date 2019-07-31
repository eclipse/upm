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
