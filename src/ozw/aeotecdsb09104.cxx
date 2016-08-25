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

#include "aeotecdsb09104.hpp"

using namespace upm;
using namespace std;


AeotecDSB09104::AeotecDSB09104(int nodeID) :
  ozwInterface(nodeID)
{
}

AeotecDSB09104::~AeotecDSB09104()
{
}

float AeotecDSB09104::getWattsC1()
{
  return m_instance->getValueAsFloat(m_nodeID, INDEX_PowerC1);
}

float AeotecDSB09104::getWattsC2()
{
  return m_instance->getValueAsFloat(m_nodeID, INDEX_PowerC2);
}

float AeotecDSB09104::getWattsC3()
{
  return m_instance->getValueAsFloat(m_nodeID, INDEX_PowerC3);
}

float AeotecDSB09104::getEnergyC1()
{
  return m_instance->getValueAsFloat(m_nodeID, INDEX_EnergyC1);
}

float AeotecDSB09104::getEnergyC2()
{
  return m_instance->getValueAsFloat(m_nodeID, INDEX_EnergyC2);
}

float AeotecDSB09104::getEnergyC3()
{
  return m_instance->getValueAsFloat(m_nodeID, INDEX_EnergyC3);
}

int AeotecDSB09104::getBatteryLevel()
{
  return (int)m_instance->getValueAsByte(m_nodeID, INDEX_BatteryLevel);
}
void AeotecDSB09104::update()
{
  m_instance->refreshValue(m_nodeID, INDEX_PowerC1);
  m_instance->refreshValue(m_nodeID, INDEX_PowerC2);
  m_instance->refreshValue(m_nodeID, INDEX_PowerC3);
  m_instance->refreshValue(m_nodeID, INDEX_EnergyC1);
  m_instance->refreshValue(m_nodeID, INDEX_EnergyC2);
  m_instance->refreshValue(m_nodeID, INDEX_EnergyC3);
  m_instance->refreshValue(m_nodeID, INDEX_BatteryLevel);
}
