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
#include <unistd.h>
#include <stdlib.h>

#include "mpu9150.h"

using namespace upm;
using namespace std;

MPU9150::MPU9150 (int bus, int address, int magAddress, bool enableAk8975) :
  m_mag(0), MPU60X0(bus, address)
{
  m_magAddress = magAddress;
  m_i2cBus = bus;
  m_enableAk8975 = enableAk8975;
}

MPU9150::~MPU9150()
{
  if (m_mag)
    delete m_mag;
}

bool MPU9150::init()
{
  // init the gyro/accel component
  if (!MPU60X0::init())
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": Unable to init MPU60X0");
      return false;
    }

  // Enabling I2C bypass will allow us to access the
  // AK8975 Magnetometer on I2C addr 0x0c.
  if (m_enableAk8975 == true)
    {
      if (!enableI2CBypass(true))
        {
          throw std::runtime_error(std::string(__FUNCTION__) +
                                   ": Unable to enable I2C bypass");
          return false;
        }

      // Now that we've done that, create an AK8975 instance and
      // initialize it.

      m_mag = new AK8975(m_i2cBus, m_magAddress);

      if (!m_mag->init())
        {
          throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": Unable to init magnetometer");
          delete m_mag;
          m_mag = 0;
          return false;
        }
    }

  return true;
}

void MPU9150::update()
{
  MPU60X0::update();

  if (m_mag)
    m_mag->update();
}

void MPU9150::getMagnetometer(float *x, float *y, float *z)
{
  float mx, my, mz;

  if (!m_mag)
    mx = my = mz = 0.0;
  else
    m_mag->getMagnetometer(&mx, &my, &mz);
  
  if (x)
    *x = mx;
  if (y)
    *y = my;
  if (z)
    *z = mz;
}

#ifdef SWIGJAVA
float *MPU9150::getMagnetometer()
{
    float *v = new float[3];
    getMagnetometer(&v[0], &v[1], &v[2]);
    return v;
}
#endif
