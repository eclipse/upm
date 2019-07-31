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
#include <errno.h>
#include <iostream>
#include <stdexcept>
#include <string>

#include "tb7300.hpp"

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


TB7300::TB7300(uint32_t targetDeviceObjectID) :
  BACNETUTIL(targetDeviceObjectID)
{
  setDebug(false);

  // we disable this by default for performance reasons
  checkReliability(false);

  m_isTempInitialized = false;
  m_isCelsius = false;

  // room temperature only
  m_temperature = 0.0;
}

TB7300::~TB7300()
{
}

void TB7300::update()
{
  if (!m_isTempInitialized)
    {
      // this will update internals so conversions work properly
      getTemperatureScale();
    }

  float tmpF = getAnalogValue(AV_Room_Temperature);

  if (m_isCelsius)
    m_temperature = tmpF;
  else
    m_temperature = f2c(tmpF);
}

float TB7300::getTemperature(bool fahrenheit)
{
  if (fahrenheit)
    return c2f(m_temperature);
  else
    return m_temperature;
}

void TB7300::setTemperatureScale(bool fahrenheit)
{
  setBinaryValue(BV_Temperature_Scale, fahrenheit);

  m_isTempInitialized = true;
  m_isCelsius = (fahrenheit) ? false : true;
}

bool TB7300::getTemperatureScale()
{
  bool scale = getBinaryValue(BV_Temperature_Scale);

  m_isTempInitialized = true;
  m_isCelsius = !scale;

  return scale;
}
