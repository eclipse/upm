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

#include <iostream>
#include <stdexcept>

#include "mma7361.hpp"

using namespace upm;
using namespace std;

MMA7361::MMA7361(int x_pin, int y_pin, int z_pin,
                 int selftest_pin, int sleep_pin,
                 int freefall_pin, int range_pin,
                 float a_ref) :
  m_mma7361(mma7361_init(x_pin, y_pin, z_pin, selftest_pin, sleep_pin,
                         freefall_pin, range_pin, a_ref))
{
  if (!m_mma7361)
    throw std::runtime_error(string(__FUNCTION__)
                             + ": mma7361_init() failed");
}

MMA7361::~MMA7361()
{
  mma7361_close(m_mma7361);
}

void MMA7361::setRange(bool range)
{
  mma7361_set_range(m_mma7361, range);
}

void MMA7361::setSleep(bool sleep)
{
  mma7361_sleep(m_mma7361, sleep);
}

bool MMA7361::isInFreefall()
{
  bool freefall;
  upm_result_t rv;
  
  if ((rv = mma7361_freefall(m_mma7361, &freefall)))
    {
      throw std::runtime_error(string(__FUNCTION__)
                               + ": mma7361_freefall() failed with UPM error "
                               + std::to_string(int(rv)) );
    }

  return freefall;
}

void MMA7361::enableSelftest(bool enable)
{
  upm_result_t rv;
  
  if ((rv = mma7361_selftest(m_mma7361, enable)))
    {
      throw std::runtime_error(string(__FUNCTION__)
                               + ": mma7361_selftest() failed with UPM error "
                               + std::to_string(int(rv)) );
    }
}

void MMA7361::update()
{
  upm_result_t rv;
  
  if ((rv = mma7361_update(m_mma7361)))
    {
      throw std::runtime_error(string(__FUNCTION__)
                               + ": mma7361_update() failed with UPM error "
                               + std::to_string(int(rv)) );
    }
}

void MMA7361::setOffset(float x, float y, float z)
{
  mma7361_set_offset(m_mma7361, x, y, z);
}

void MMA7361::setScale(float x, float y, float z)
{
  mma7361_set_scale(m_mma7361, x, y, z);
}

void MMA7361::getAcceleration(float *x, float *y, float *z)
{
  mma7361_get_acceleration(m_mma7361, x, y, z);
}

float *MMA7361::getAcceleration()
{
  static float data[3];

  getAcceleration(&data[0], &data[1], &data[2]);

  return data;
}

void MMA7361::getVolts(float *x, float *y, float *z)
{
  mma7361_get_volts(m_mma7361, x, y, z);
}

float *MMA7361::getVolts()
{
  static float data[3];

  getVolts(&data[0], &data[1], &data[2]);

  return data;
}

void MMA7361::getNormalized(float *x, float *y, float *z)
{
  mma7361_get_normalized(m_mma7361, x, y, z);
}

float *MMA7361::getNormalized()
{
  static float data[3];

  getNormalized(&data[0], &data[1], &data[2]);

  return data;
}
