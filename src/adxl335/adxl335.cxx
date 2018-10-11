/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * Adapted from the seeedstudio example
 * https://github.com/Seeed-Studio/Accelerometer_ADXL335
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
#include <string>
#include <stdexcept>

#include "adxl335.hpp"

using namespace std;
using namespace upm;

ADXL335::ADXL335(int pinX, int pinY, int pinZ, float aref)
{
  m_aref = aref;
  m_zeroX = 0.0;
  m_zeroY = 0.0;
  m_zeroZ = 0.0;

  if ( !(m_aioX = mraa_aio_init(pinX)) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_aio_init(X) failed, invalid pin?");
      return;
    }

  if ( !(m_aioY = mraa_aio_init(pinY)) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_aio_init(Y) failed, invalid pin?");
      return;
    }

  if ( !(m_aioZ = mraa_aio_init(pinZ)) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_aio_init(Z) failed, invalid pin?");
      return;
    }
}

ADXL335::~ADXL335()
{
  mraa_aio_close(m_aioX);
  mraa_aio_close(m_aioY);
  mraa_aio_close(m_aioZ);
}

void ADXL335::values(int *xVal, int *yVal, int *zVal)
{
  *xVal = mraa_aio_read(m_aioX);
  *yVal = mraa_aio_read(m_aioY);
  *zVal = mraa_aio_read(m_aioZ);
}

std::vector<int> ADXL335::values()
{
  std::vector<int> v(3);
  values(&v[0], &v[1], &v[2]);
  return v;
}

void ADXL335::acceleration(float *xAccel, float *yAccel, float *zAccel)
{
  int x, y, z;
  float xVolts, yVolts, zVolts;

  values(&x, &y, &z);
  xVolts = float(x) * m_aref / 1024.0;
  yVolts = float(y) * m_aref / 1024.0;
  zVolts = float(z) * m_aref / 1024.0;

  *xAccel = (xVolts - m_zeroX) / ADXL335_SENSITIVITY;
  *yAccel = (yVolts - m_zeroY) / ADXL335_SENSITIVITY;
  *zAccel = (zVolts - m_zeroZ) / ADXL335_SENSITIVITY;
}

std::vector<float> ADXL335::acceleration()
{
  std::vector<float> v(3);
  acceleration(&v[0], &v[1], &v[2]);
  return v;
}

std::vector<float> ADXL335::getAcceleration()
{
  std::vector<float> v(3);

  int x, y, z;
  float xVolts, yVolts, zVolts;

  x = mraa_aio_read(m_aioX);
  y = mraa_aio_read(m_aioY);
  z = mraa_aio_read(m_aioZ);
  
  xVolts = float(x) * m_aref / 1024.0;
  yVolts = float(y) * m_aref / 1024.0;
  zVolts = float(z) * m_aref / 1024.0;

  v[0] = (xVolts - m_zeroX) / ADXL335_SENSITIVITY;
  v[1] = (yVolts - m_zeroY) / ADXL335_SENSITIVITY;
  v[2] = (zVolts - m_zeroZ) / ADXL335_SENSITIVITY;

  return v;
}

void ADXL335::calibrate()
{
  // make sure the sensor is still before running calibration.

  int x, y, z;
  usleep(10000);

  values(&x, &y, &z);

  setZeroX(float(x) * m_aref / 1024.0);
  setZeroY(float(y) * m_aref / 1024.0);
  setZeroZ(float(z) * m_aref / 1024.0);
}
