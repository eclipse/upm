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

#include "aeotecss6.hpp"

using namespace upm;
using namespace std;


AeotecSS6::AeotecSS6(int nodeID) :
  ozwInterface(nodeID)
{
}

AeotecSS6::~AeotecSS6()
{
}

void AeotecSS6::on()
{
  m_instance->setValueAsBool(m_nodeID, INDEX_SwitchCtrl, true);
}

void AeotecSS6::off()
{
  m_instance->setValueAsBool(m_nodeID, INDEX_SwitchCtrl, false);
}

bool AeotecSS6::isOn()
{
  return m_instance->getValueAsBool(m_nodeID, INDEX_SwitchCtrl);
}

float AeotecSS6::getEnergy()
{
  return m_instance->getValueAsFloat(m_nodeID, INDEX_Energy_kWh);
}

float AeotecSS6::getWatts()
{
  return m_instance->getValueAsFloat(m_nodeID, INDEX_Power_Watts);
}

float AeotecSS6::getVolts()
{
  return m_instance->getValueAsFloat(m_nodeID, INDEX_Volts);
}

float AeotecSS6::getCurrent()
{
  return m_instance->getValueAsFloat(m_nodeID, INDEX_Current);
}

void AeotecSS6::update()
{
  m_instance->refreshValue(m_nodeID, INDEX_SwitchCtrl);
  m_instance->refreshValue(m_nodeID, INDEX_Energy_kWh);
  m_instance->refreshValue(m_nodeID, INDEX_Power_Watts);
  m_instance->refreshValue(m_nodeID, INDEX_Volts);
  m_instance->refreshValue(m_nodeID, INDEX_Current);
}
