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
