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
#include <math.h>
#include <iostream>
#include <stdexcept>
#include <string>

#include "h3lis331dl.h"

using namespace upm;
using namespace std;


H3LIS331DL::H3LIS331DL(int bus, uint8_t address):
  m_i2c(bus)
{
  m_addr = address;

  mraa::Result rv;
  if ( (rv = m_i2c.address(m_addr)) != mraa::SUCCESS)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.address() failed");
      return;
    }

  m_rawX = m_rawY = m_rawZ = 0;
  setAdjustmentOffsets(0, 0, 0);
}

H3LIS331DL::~H3LIS331DL()
{
}

bool H3LIS331DL::init(DR_BITS_T odr, PM_BITS_T pm, FS_BITS_T fs)
{
  if (!setDataRate(odr))
    return false;
  if (!setPowerMode(pm))
    return false;
  if (!setFullScale(fs))
    return false;

  // now enable X, Y, and Z axes
  if (enableAxis(REG1_XEN | REG1_YEN | REG1_ZEN))
    return false;

  return true;
}

uint8_t H3LIS331DL::getChipID()
{
  return m_i2c.readReg(REG_WHOAMI);
}

bool H3LIS331DL::setDataRate(DR_BITS_T odr)
{
  uint8_t reg1 = m_i2c.readReg(REG_REG1);

  reg1 &= ~(REG1_DR0 | REG1_DR1);
  reg1 |= (odr << REG1_DR_SHIFT);

  if (m_i2c.writeReg(REG_REG1, reg1))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.writeReg() failed");
      return false;
    }

  return true;
}

bool H3LIS331DL::setPowerMode(PM_BITS_T pm)
{
  uint8_t reg1 = m_i2c.readReg(REG_REG1);

  reg1 &= ~(REG1_PM0 | REG1_PM1 | REG1_PM2);
  reg1 |= (pm << REG1_PM_SHIFT);

  if (m_i2c.writeReg(REG_REG1, reg1))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.writeReg() failed");
      return false;
    }

  return true;
}

bool H3LIS331DL::enableAxis(uint8_t axisEnable)
{
  uint8_t reg1 = m_i2c.readReg(REG_REG1);

  reg1 &= ~(REG1_XEN | REG1_YEN | REG1_ZEN);
  reg1 |= (axisEnable & (REG1_XEN | REG1_YEN | REG1_ZEN));

  if (m_i2c.writeReg(REG_REG1, reg1))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.writeReg() failed");
      return false;
    }

  return true;
}

bool H3LIS331DL::setFullScale(FS_BITS_T fs)
{
  uint8_t reg4 = m_i2c.readReg(REG_REG4);

  reg4 &= ~(REG4_FS0 | REG4_FS1);
  reg4 |= (fs << REG4_FS_SHIFT);

  if (m_i2c.writeReg(REG_REG4, reg4))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.writeReg() failed");
      return false;
    }

  return true;
}

bool H3LIS331DL::setHPCF(HPCF_BITS_T val)
{
  uint8_t reg = m_i2c.readReg(REG_REG2);

  reg &= ~(REG2_HPCF0 | REG2_HPCF1);
  reg |= (val << REG2_HPCF_SHIFT);

  if (m_i2c.writeReg(REG_REG2, reg))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.writeReg() failed");
      return false;
    }

  return true;
}

bool H3LIS331DL::setHPM(HPM_BITS_T val)
{
  uint8_t reg = m_i2c.readReg(REG_REG2);

  reg &= ~(REG2_HPM0 | REG2_HPM1);
  reg |= (val << REG2_HPM_SHIFT);

  if (m_i2c.writeReg(REG_REG2, reg))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.writeReg() failed");
      return false;
    }

  return true;
}

bool H3LIS331DL::boot()
{
  uint8_t reg = m_i2c.readReg(REG_REG2);

  reg |= REG2_BOOT;

  if (m_i2c.writeReg(REG_REG2, reg))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.writeReg() failed");
      return false;
    }

  // wait for the boot bit to clear
  do {
    reg = m_i2c.readReg(REG_REG2);
    usleep(200000);
  } while (reg & REG2_BOOT);

  return true;
}

bool H3LIS331DL::enableHPF1(bool enable)
{
  uint8_t reg = m_i2c.readReg(REG_REG2);

  if (enable)
    reg |= REG2_HPEN1;
  else
    reg &= ~REG2_HPEN1;

  if (m_i2c.writeReg(REG_REG2, reg))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.writeReg() failed");
      return false;
    }

  return true;
}

bool H3LIS331DL::enableHPF2(bool enable)
{
  uint8_t reg = m_i2c.readReg(REG_REG2);

  if (enable)
    reg |= REG2_HPEN2;
  else
    reg &= ~REG2_HPEN2;

  if (m_i2c.writeReg(REG_REG2, reg))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.writeReg() failed");
      return false;
    }

  return true;
}

bool H3LIS331DL::enableFDS(bool enable)
{
  uint8_t reg = m_i2c.readReg(REG_REG2);

  if (enable)
    reg |= REG2_FDS;
  else
    reg &= ~REG2_FDS;

  if (m_i2c.writeReg(REG_REG2, reg))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.writeReg() failed");
      return false;
    }

  return true;
}

bool H3LIS331DL::setInterruptActiveLow(bool enable)
{
  uint8_t reg = m_i2c.readReg(REG_REG3);

  if (enable)
    reg |= REG3_IHL;
  else
    reg &= ~REG3_IHL;

  if (m_i2c.writeReg(REG_REG3, reg))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.writeReg() failed");
      return false;
    }

  return true;
}

bool H3LIS331DL::setInterruptOpenDrain(bool enable)
{
  uint8_t reg = m_i2c.readReg(REG_REG3);

  if (enable)
    reg |= REG3_PP_OD;
  else
    reg &= ~REG3_PP_OD;

  if (m_i2c.writeReg(REG_REG3, reg))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.writeReg() failed");
      return false;
    }

  return true;
}

bool H3LIS331DL::setInterrupt1Latch(bool enable)
{
  uint8_t reg = m_i2c.readReg(REG_REG3);

  if (enable)
    reg |= REG3_LIR1;
  else
    reg &= ~REG3_LIR1;

  if (m_i2c.writeReg(REG_REG3, reg))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.writeReg() failed");
      return false;
    }

  return true;
}

bool H3LIS331DL::setInterrupt2Latch(bool enable)
{
  uint8_t reg = m_i2c.readReg(REG_REG3);

  if (enable)
    reg |= REG3_LIR2;
  else
    reg &= ~REG3_LIR2;

  if (m_i2c.writeReg(REG_REG3, reg))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.writeReg() failed");
      return false;
    }

  return true;
}

bool H3LIS331DL::setInterrupt1PadConfig(I_CFG_BITS_T val)
{
  uint8_t reg = m_i2c.readReg(REG_REG3);

  reg &= ~(REG3_I1_CFG0 | REG3_I1_CFG1);
  reg |= (val << REG3_I1_CFG_SHIFT);

  if (m_i2c.writeReg(REG_REG3, reg))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.writeReg() failed");
      return false;
    }

  return true;
}

bool H3LIS331DL::setInterrupt2PadConfig(I_CFG_BITS_T val)
{
  uint8_t reg = m_i2c.readReg(REG_REG3);

  reg &= ~(REG3_I2_CFG0 | REG3_I2_CFG1);
  reg |= (val << REG3_I2_CFG_SHIFT);

  if (m_i2c.writeReg(REG_REG3, reg))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.writeReg() failed");
      return false;
    }

  return true;
}


bool H3LIS331DL::enableBDU(bool enable)
{
  uint8_t reg = m_i2c.readReg(REG_REG4);

  if (enable)
    reg |= REG4_BDU;
  else
    reg &= ~REG4_BDU;

  if (m_i2c.writeReg(REG_REG4, reg))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.writeReg() failed");
      return false;
    }

  return true;
}

bool H3LIS331DL::enableBLE(bool enable)
{
  uint8_t reg = m_i2c.readReg(REG_REG4);

  if (enable)
    reg |= REG4_BLE;
  else
    reg &= ~REG4_BLE;

  if (m_i2c.writeReg(REG_REG4, reg))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.writeReg() failed");
      return false;
    }

  return true;
}

bool H3LIS331DL::enableSleepToWake(bool enable)
{
  uint8_t reg = m_i2c.readReg(REG_REG5);

  if (enable)
    reg |= (REG5_TURNON0 | REG5_TURNON1);
  else
    reg &= ~(REG5_TURNON0 | REG5_TURNON1);

  if (m_i2c.writeReg(REG_REG5, reg))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.writeReg() failed");
      return false;
    }

  return true;
}

uint8_t H3LIS331DL::getStatus()
{
  return m_i2c.readReg(REG_STATUS);
}

bool H3LIS331DL::setInterrupt1Config(uint8_t val)
{
  uint8_t reg = m_i2c.readReg(REG_INT1_CFG);

  // mask off reserved bit
  reg = (val & ~0x40);

  if (m_i2c.writeReg(REG_INT1_CFG, reg))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.writeReg() failed");
      return false;
    }

  return true;
}

bool H3LIS331DL::setInterrupt1Source(uint8_t val)
{
  uint8_t reg = m_i2c.readReg(REG_INT1_SRC);

  // mask off reserved bit
  reg = (val & ~0x80);

  if (m_i2c.writeReg(REG_INT1_SRC, reg))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.writeReg() failed");
      return false;
    }

  return true;
}

bool H3LIS331DL::setInterrupt1Threshold(uint8_t val)
{
  if (m_i2c.writeReg(REG_INT1_THS, val))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.writeReg() failed");
      return false;
    }

  return true;
}

bool H3LIS331DL::setInterrupt1Duration(uint8_t val)
{
  if (m_i2c.writeReg(REG_INT1_DUR, val))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.writeReg() failed");
      return false;
    }

  return true;
}

bool H3LIS331DL::setInterrupt2Config(uint8_t val)
{
  uint8_t reg = m_i2c.readReg(REG_INT2_CFG);

  // mask off reserved bit
  reg = (val & ~0x40);

  if (m_i2c.writeReg(REG_INT2_CFG, reg))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.writeReg() failed");
      return false;
    }

  return true;
}

bool H3LIS331DL::setInterrupt2Source(uint8_t val)
{
  uint8_t reg = m_i2c.readReg(REG_INT2_SRC);

  // mask off reserved bit
  reg = (val & ~0x80);

  if (m_i2c.writeReg(REG_INT2_SRC, reg))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.writeReg() failed");
      return false;
    }

  return true;
}

bool H3LIS331DL::setInterrupt2Threshold(uint8_t val)
{
  if (m_i2c.writeReg(REG_INT2_THS, val))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.writeReg() failed");
      return false;
    }

  return true;
}

bool H3LIS331DL::setInterrupt2Duration(uint8_t val)
{
  if (m_i2c.writeReg(REG_INT2_DUR, val))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.writeReg() failed");
      return false;
    }

  return true;
}

void H3LIS331DL::update()
{
  uint8_t low, high;

  // X
  low = m_i2c.readReg(REG_OUT_X_L);
  high = m_i2c.readReg(REG_OUT_X_H);
  m_rawX = ((high << 8) | low);

  // Y
  low = m_i2c.readReg(REG_OUT_Y_L);
  high = m_i2c.readReg(REG_OUT_Y_H);
  m_rawY = ((high << 8) | low);

  // Z
  low = m_i2c.readReg(REG_OUT_Z_L);
  high = m_i2c.readReg(REG_OUT_Z_H);
  m_rawZ = ((high << 8) | low);
}

void H3LIS331DL::setAdjustmentOffsets(int adjX, int adjY, int adjZ)
{
  m_adjX = adjX;
  m_adjY = adjY;
  m_adjZ = adjZ;
}

void H3LIS331DL::getAcceleration(float *aX, float *aY, float *aZ)
{
  const float gains = 0.003;    // Seeed magic number?

  *aX = float(m_rawX - m_adjX) * gains;
  *aY = float(m_rawY - m_adjY) * gains;
  *aZ = float(m_rawZ - m_adjZ) * gains;
}

void H3LIS331DL::getRawXYZ(int *x, int *y, int*z)
{
  *x = m_rawX;
  *y = m_rawY;
  *z = m_rawZ;
}

void H3LIS331DL::getXYZ(int *x, int *y, int*z)
{
  *x = (m_rawX - m_adjX);
  *y = (m_rawY - m_adjY);
  *z = (m_rawZ - m_adjZ);
}

#ifdef SWIGJAVA
float *H3LIS331DL::getAcceleration()
{
  float *v = new float[3];
  getAcceleration(&v[0], &v[1], &v[2]);
  return v;
}

int *H3LIS331DL::getRawXYZ()
{
  int *v = new int[3];
  getRawXYZ(&v[0], &v[1], &v[2]);
  return v;
}

int *H3LIS331DL::getXYZ()
{
  int *v = new int[3];
  getXYZ(&v[0], &v[1], &v[2]);
  return v;
}
#endif
