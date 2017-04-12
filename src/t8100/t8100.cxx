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
#include <assert.h>
#include <errno.h>
#include <iostream>
#include <stdexcept>
#include <string>

#include "t8100.hpp"

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


T8100::T8100(uint32_t targetDeviceObjectID) :
  BACNETUTIL(targetDeviceObjectID)
{
  setDebug(false);

  // we disable this by default for performance reasons
  checkReliability(false);

  m_isTempInitialized = false;
  m_isCelsius = false;

  m_humidity = 0.0;
  m_temperature = 0.0;
  m_co2 = 0.0;
  m_relayState = false;
}

T8100::~T8100()
{
}

void T8100::update()
{
  if (!m_isTempInitialized)
    {
      // this will update internals so conversions work properly
      getTemperatureScale();
    }

  float tmpF = getAnalogInput(AI_Temperature_Thermistor);

  if (m_isCelsius)
    m_temperature = tmpF;
  else
    m_temperature = f2c(tmpF);

  m_humidity = getAnalogInput(AI_Relative_Humidity);
  m_co2 = getAnalogInput(AI_CO2);
  m_relayState = getBinaryInput(BI_Relay_State);
}

float T8100::getTemperature(bool fahrenheit)
{
  if (fahrenheit)
    return c2f(m_temperature);
  else
    return m_temperature;
}

void T8100::setTemperatureScale(bool fahrenheit)
{
  setBinaryValue(BV_Temperature_Units, fahrenheit);

  m_isTempInitialized = true;
  m_isCelsius = (fahrenheit) ? false : true;
}

bool T8100::getTemperatureScale()
{
  bool scale = getBinaryValue(BV_Temperature_Units);

  m_isTempInitialized = true;
  m_isCelsius = !scale;

  return scale;
}

float T8100::getTemperatureOffset()
{
  return getAnalogValue(AV_Temperature_Offset);
}

void T8100::setTemperatureOffset(float value)
{
  // Always in C...
  if (value < -50.0 || value > 50.0)
    {
      throw std::out_of_range(std::string(__FUNCTION__)
                              + ": value must be between -50 and 50,"
                              + " in degrees Celsius");

    }

  setAnalogValue(AV_Temperature_Offset, value);
}

float T8100::getHumidityOffset()
{
  return getAnalogValue(AV_RH_Offset);
}

void T8100::setHumidityOffset(float value)
{
  if (value < -100.0 || value > 100.0)
    {
      throw std::out_of_range(std::string(__FUNCTION__)
                              + ": value must be between -100 and 100");
    }

  setAnalogValue(AV_RH_Offset, value);
}

float T8100::getRelaySetPoint()
{
  return getAnalogValue(AV_Relay_Set_Point);
}

void T8100::setRelaySetPoint(float value)
{
  if (value < 0.00 || value > 65535.0)
    {
      throw std::out_of_range(std::string(__FUNCTION__)
                              + ": value must be between 0 and 65535");
    }

  setAnalogValue(AV_Relay_Set_Point, value);
}

float T8100::getRelayHysteresis()
{
  return getAnalogValue(AV_Relay_Hysteresis);
}

void T8100::setRelayHysteresis(float value)
{
  if (value < 0.00 || value > 65535.0)
    {
      throw std::out_of_range(std::string(__FUNCTION__)
                              + ": value must be between 0 and 65535");
    }

  setAnalogValue(AV_Relay_Hysteresis, value);
}

float T8100::getElevation()
{
  return getAnalogValue(AV_Elevation);
}

void T8100::setElevation(float value)
{
  if (value < 0.00 || value > 65535.0)
    {
      throw std::out_of_range(std::string(__FUNCTION__)
                              + ": value must be between 0 and 65535");
    }

  setAnalogValue(AV_Elevation, value);
}

float T8100::getCalibrationSinglePoint()
{
  return getAnalogValue(AV_Calibration_Single_Point);
}

void T8100::setCalibrationSinglePoint(float value)
{
  if (value < 0.00 || value > 65535.0)
    {
      throw std::out_of_range(std::string(__FUNCTION__)
                              + ": value must be between 0 and 65535");
    }

  setAnalogValue(AV_Calibration_Single_Point, value);
}

float T8100::getBaudRate()
{
  return getAnalogValue(AV_Baud_Rate);
}

float T8100::getMACAddress()
{
  return getAnalogValue(AV_MAC_Address);
}

bool T8100::getABCLogicState()
{
  return getBinaryValue(BV_ABC_Logic_State);
}

void T8100::setABCLogicState(bool value)
{
  setBinaryValue(BV_ABC_Logic_State, value);
}

bool T8100::getABCLogicReset()
{
  return getBinaryValue(BV_ABC_Logic_Reset);
}

void T8100::setABCLogicReset(bool value)
{
  setBinaryValue(BV_ABC_Logic_Reset, value);
}

bool T8100::getCO2Calibration()
{
  return getBinaryValue(BV_CO2_Calibration);
}

void T8100::setCO2Calibration(bool value)
{
  setBinaryValue(BV_CO2_Calibration, value);
}
