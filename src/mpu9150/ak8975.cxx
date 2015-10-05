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

#include <unistd.h>
#include <iostream>
#include <string>
#include <stdexcept>

#include "ak8975.h"

using namespace upm;
using namespace std;


AK8975::AK8975(int bus, uint8_t address):
  m_i2c(bus)
{
  m_addr = address;
  m_xCoeff = 0.0;
  m_yCoeff = 0.0;
  m_zCoeff = 0.0;

  mraa::Result rv;
  if ( (rv = m_i2c.address(m_addr)) != mraa::SUCCESS)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.address() failed");
      return;
    }
}

AK8975::~AK8975()
{
}

bool AK8975::init()
{
  // we put the device in 'fuse mode', and then read the compensation
  // coefficients and store them.

  // first, set power down mode

  if (!setMode(CNTL_PWRDWN))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": Unable to set PWRDWN mode");
      return false;
    }

  if (!setMode(CNTL_FUSE_ACCESS))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": Unable to set FUSE mode");
      return false;
    }

  // Read each byte and store
  m_xCoeff = (float)m_i2c.readReg(REG_ASAX);
  m_yCoeff = (float)m_i2c.readReg(REG_ASAY);
  m_zCoeff = (float)m_i2c.readReg(REG_ASAZ);

  // now, place back in power down mode
  if (!setMode(CNTL_PWRDWN))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": Unable to set reset PWRDWN mode");
      return false;
    }

  return true;
}

bool AK8975::setMode(CNTL_MODES_T mode)
{
  mraa::Result rv;
  if ((rv = m_i2c.writeReg(REG_CNTL, mode)) != mraa::SUCCESS)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.writeReg() failed");
      return false;
    } 

  // sleep at least 100us for for mode transition to complete
  usleep(150);

  return true;
}

bool AK8975::isReady()
{
  uint8_t rdy = m_i2c.readReg(REG_ST1);
  
  if (rdy & ST1_DRDY)
    return true;

  return false;
}

bool AK8975::waitforDeviceReady()
{
  const int maxRetries = 20;

  int retries = 0;

  while (retries < maxRetries)
    {
      if (isReady())
        return true;

      usleep(5000);
      retries++;
    }
  
  throw std::runtime_error(std::string(__FUNCTION__) +
                           ": timeout waiting for device to become ready");

  return false;
}

bool AK8975::update(bool selfTest)
{
  // this flag (selfTest) is used so that we can read values without
  // specifically taking a measurement.  For example, selfTest will
  // pass true to this method so that the test results aren't
  // overwritten by a measurement.
  if (!selfTest)
    {
      // First set measurement mode (take a measurement)
      if (!setMode(CNTL_MEASURE))
        {
          throw std::runtime_error(std::string(__FUNCTION__) +
                                   ": Unable to set MEASURE mode");
          return false;
        }
    }

  if (!waitforDeviceReady())
    return false;

  // hope it worked.  Now read out the values and store them (uncompensated)
  uint8_t data[6];
  m_i2c.readBytesReg(REG_HXL, data, 6);

  int16_t x, y, z;
  x = ( (data[1] << 8) | data[0] );
  y = ( (data[3] << 8) | data[2] );
  z = ( (data[5] << 8) | data[4] );

  m_xData = float(x);
  m_yData = float(y);
  m_zData = float(z);

  return true;
}

bool AK8975::selfTest()
{
  mraa::Result rv;

  // set power down first
  if (!setMode(CNTL_PWRDWN))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": Unable to set PWRDWN mode");
      return false;
    }

  // enable self test bit
  if ((rv = m_i2c.writeReg(REG_ASTC, ASTC_SELF)) != mraa::SUCCESS)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": failed to enable self test");
      return false;
    } 
  
  // now set self test mode
  if (!setMode(CNTL_SELFTEST))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": Unable to set SELFTEST mode");
      return false;
    }
  
  // now update current data (without enabling a measurement)
  update(true);
  
  // Now, reset self test register
  uint8_t reg = m_i2c.readReg(REG_ASTC);
  reg &= ~ASTC_SELF;
  if ((rv = m_i2c.writeReg(REG_ASTC, reg)) != mraa::SUCCESS)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": failed to disable self test");
      return false;
    } 

  // after self-test measurement, device transitions to power down mode
  return true;
}

float AK8975::adjustValue(float value, float adj)
{
  // apply the proper compensation to value.  This equation is taken
  // from the AK8975 datasheet, section 8.3.11

  return ( value * ((((adj - 128.0) * 0.5) / 128.0) + 1.0) );
} 

void AK8975::getMagnetometer(float *x, float *y, float *z)
{
  if (x)
    *x = adjustValue(m_xData, m_xCoeff);
  if (y)
    *y = adjustValue(m_yData, m_yCoeff);
  if (z)
    *z = adjustValue(m_zData, m_zCoeff);
}

