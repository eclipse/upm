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
