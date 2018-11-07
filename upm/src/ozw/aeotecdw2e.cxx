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
