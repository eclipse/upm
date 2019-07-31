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

#include "aeotecdw2e.hpp"

using namespace upm;
using namespace std;


AeotecDW2E::AeotecDW2E(int nodeID) :
  ozwInterface(nodeID)
{
}

AeotecDW2E::~AeotecDW2E()
{
}

bool AeotecDW2E::isDeviceAvailable()
{
  return m_instance->isNodeInfoReceived(m_nodeID);

}

bool AeotecDW2E::isAlarmTripped()
{
  if (isDeviceAvailable())
    return m_instance->getValueAsBool(m_nodeID, INDEX_Alarm);
  else
    return false;
}

bool AeotecDW2E::isTamperTripped()
{
  if (isDeviceAvailable())
    {
      if (m_instance->getValueAsByte(m_nodeID, INDEX_AlarmLevel) > 0)
        return true;
      else
        return false;
    }
  else
    return false;
}

int AeotecDW2E::getBatteryLevel()
{
  if (isDeviceAvailable())
    return (int)m_instance->getValueAsByte(m_nodeID, INDEX_BatteryLevel);
  else
    return 0;
}
