/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
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

#include <iostream>

#include "adxrs610.h"

using namespace std;
using namespace upm;

ADXRS610::ADXRS610(int dPin, int tPin, float aref) :
  m_aioData(dPin), m_aioTemp(tPin)
{
  // ADC resolution of data and temp should be the same...
  m_aRes = (1 << m_aioData.getBit());
  m_aref = aref;

  setZeroPoint(calibrateZeroPoint());
  setDeadband(0.0);

  m_centerVolts = aref / 2.0;

}

ADXRS610::~ADXRS610()
{
}

float ADXRS610::getDataVolts()
{
  int val = m_aioData.read();

  return(float(val) * (m_aref / float(m_aRes)));
}

float ADXRS610::getTemperatureVolts()
{
  int val = m_aioTemp.read();

  return(float(val) * (m_aref / float(m_aRes)));
}

float ADXRS610::calibrateZeroPoint(unsigned int samples)
{
  // The gyro should be in a stable, non-moving state

  float sum = 0;
  for (int i=0; i<samples; i++)
    sum += getDataVolts();

  return sum / samples;
}

float ADXRS610::getTemperature()
{
  float tempV = getTemperatureVolts();
  
  // nominal 2.5 volts at 25C
  if (tempV > m_centerVolts)
    return (m_temperatureNom + ((tempV - m_centerVolts) / m_temperatureCoeff));
  else
    return (m_temperatureNom - ((m_centerVolts - tempV) / m_temperatureCoeff));
}

float ADXRS610::getAngularVelocity()
{
  float dataV = getDataVolts();

  // check the deadband
  if (dataV < (m_zeroPoint + m_deadband) &&
      dataV > (m_zeroPoint - m_deadband))
    return 0.0;
  
  if (dataV > m_zeroPoint)
    return ((dataV - m_zeroPoint) / m_degreeCoeff);
  else
    return -((m_zeroPoint - dataV) / m_degreeCoeff);
}
