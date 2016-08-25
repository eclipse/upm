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
#include <string>

#include "bmx055.hpp"

using namespace upm;
using namespace std;

BMX055::BMX055(int accelBus, int accelAddr, int accelCS,
               int gyroBus, int gyroAddr, int gyroCS,
               int magBus, int magAddr, int magCS) :
  m_accel(0), m_gyro(0), m_mag(0)
{
  // if -1 is supplied as a bus for any of these, we will not
  // instantiate them

  if (accelBus >= 0)
    m_accel = new BMA250E(accelBus, accelAddr, accelCS);

  if (gyroBus >= 0)
    m_gyro = new BMG160(gyroBus, gyroAddr, gyroCS);

  if (magBus >= 0)
    m_mag = new BMM150(magBus, magAddr, magCS);

  // now initialize them...
  if (m_accel)
    m_accel->init();

  if (m_gyro)
    m_gyro->init();

  if (m_mag)
    m_mag->init();
}

BMX055::~BMX055()
{
  if (m_accel)
    delete m_accel;

  if (m_gyro)
    delete m_gyro;

  if (m_mag)
    delete m_mag;
}

void BMX055::initAccelerometer(BMA250E::POWER_MODE_T pwr,
                               BMA250E::RANGE_T range,
                               BMA250E::BW_T bw)
{
  if (m_accel)
    m_accel->init(pwr, range, bw);
}

void BMX055::initGyroscope(BMG160::POWER_MODE_T pwr,
                           BMG160::RANGE_T range,
                           BMG160::BW_T bw)
{
  if (m_gyro)
    m_gyro->init(pwr, range, bw);
}

void BMX055::initMagnetometer(BMM150::USAGE_PRESETS_T usage)
{
  if (m_mag)
    m_mag->init(usage);
}

void BMX055::update()
{
  if (m_accel)
    m_accel->update();

  if (m_gyro)
    m_gyro->update();

  if (m_mag)
    m_mag->update();
}

void BMX055::getAccelerometer(float *x, float *y, float *z)
{
  if (m_accel)
    m_accel->getAccelerometer(x, y, z);
}

float *BMX055::getAccelerometer()
{
  if (m_accel)
    return m_accel->getAccelerometer();
  else
    {
      static float v[3] = {0.0f, 0.0f, 0.0f};
      return v;
    }
}

void BMX055::getGyroscope(float *x, float *y, float *z)
{
  if (m_gyro)
    m_gyro->getGyroscope(x, y, z);
}

float *BMX055::getGyroscope()
{
  if (m_gyro)
    return m_gyro->getGyroscope();
  else
    {
      static float v[3] = {0.0f, 0.0f, 0.0f};
      return v;
    }
}

void BMX055::getMagnetometer(float *x, float *y, float *z)
{
  if (m_mag)
    m_mag->getMagnetometer(x, y, z);
}

float *BMX055::getMagnetometer()
{
  if (m_mag)
    return m_mag->getMagnetometer();
  else
    {
      static float v[3] = {0.0f, 0.0f, 0.0f};
      return v;
    }
}
