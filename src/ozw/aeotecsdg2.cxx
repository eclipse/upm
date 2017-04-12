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

#include "aeotecsdg2.hpp"

using namespace upm;
using namespace std;


AeotecSDG2::AeotecSDG2(int nodeID) :
  ozwInterface(nodeID)
{
}

AeotecSDG2::~AeotecSDG2()
{
}

void AeotecSDG2::setLevel(int level)
{
  // level must be between 0-99, so we clamp it
  if (level < 0)
    level = 0;
  if (level > 99)
    level = 99;

  m_instance->setValueAsByte(m_nodeID, INDEX_Level, level);
}

int AeotecSDG2::getLevel()
{
  return m_instance->getValueAsByte(m_nodeID, INDEX_Level);
}

bool AeotecSDG2::isOn()
{
  if (m_instance->getValueAsByte(m_nodeID, INDEX_Level) > 0)
    return true;
  else
    return false;
}

float AeotecSDG2::getEnergy()
{
  return m_instance->getValueAsFloat(m_nodeID, INDEX_Energy_kWh);
}

float AeotecSDG2::getWatts()
{
  return m_instance->getValueAsFloat(m_nodeID, INDEX_Power_Watts);
}

float AeotecSDG2::getVolts()
{
  return m_instance->getValueAsFloat(m_nodeID, INDEX_Volts);
}

float AeotecSDG2::getCurrent()
{
  return m_instance->getValueAsFloat(m_nodeID, INDEX_Current);
}

void AeotecSDG2::update()
{
  m_instance->refreshValue(m_nodeID, INDEX_Level);
  m_instance->refreshValue(m_nodeID, INDEX_Energy_kWh);
  m_instance->refreshValue(m_nodeID, INDEX_Power_Watts);
  m_instance->refreshValue(m_nodeID, INDEX_Volts);
  m_instance->refreshValue(m_nodeID, INDEX_Current);
}
