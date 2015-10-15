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
#include <stdexcept>
#include <string>
#include <string.h>

#include "bma220.h"

using namespace upm;
using namespace std;


BMA220::BMA220(int bus, uint8_t addr) :
  m_i2c(bus), m_gpioIntr(0)
{
  m_addr = addr;

  m_accelX = 0.0;
  m_accelY = 0.0;
  m_accelZ = 0.0;
  
  m_accelScale = 0.0;

  mraa::Result rv;
  if ( (rv = m_i2c.address(m_addr)) != mraa::SUCCESS)
    {
      throw std::runtime_error(string(__FUNCTION__) +
                               ": I2c.address() failed");
      return;
    }

  // Init the accelerometer
  enableAxes(true, true, true);

  // set scaling rate
  if (!setAccelerometerScale(FSL_RANGE_2G))
    {
      throw std::runtime_error(string(__FUNCTION__) + 
                               ": Unable to set accel scale");
      return;
    }
}

BMA220::~BMA220()
{
  uninstallISR();
}

void BMA220::update()
{
  updateAccelerometer();
}

void BMA220::updateAccelerometer()
{
  int x, y, z;
  char buf = 0;

  buf = (char)readReg(REG_ACC_X);
  x = int(buf) / 4;

  buf = (char)readReg(REG_ACC_Y);
  y = int(buf) / 4;

  buf = (char)readReg(REG_ACC_Z);
  z = int(buf) / 4;

  m_accelX = float(x);
  m_accelY = float(y);
  m_accelZ = float(z);
}

uint8_t BMA220::readReg(uint8_t reg)
{
  return m_i2c.readReg(reg);
}

bool BMA220::writeReg(uint8_t reg, uint8_t val)
{
  mraa::Result rv;
  if ((rv = m_i2c.writeReg(reg, val)) != mraa::SUCCESS)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.writeReg() failed");
      return false;
    } 
  
  return true;
}

bool BMA220::setAccelerometerScale(FSL_RANGE_T scale)
{
  uint8_t reg = readReg(REG_SBIST_FSL_CONFIG);

  reg &= ~(_REG_SBIST_FSL_RANGE_MASK << _REG_SBIST_FSL_RANGE_SHIFT);

  reg |= (scale << _REG_SBIST_FSL_RANGE_SHIFT);

  if (!writeReg(REG_SBIST_FSL_CONFIG, reg))
    {
      return false;
    }

  // store scaling factor
  
  switch (scale)
    {
    case FSL_RANGE_2G:
      m_accelScale = 16.0; 
      break;

    case FSL_RANGE_4G:
      m_accelScale = 8.0;
      break;

    case FSL_RANGE_8G:
      m_accelScale = 4.0;
      break;

    case FSL_RANGE_16G:
      m_accelScale = 2.0;
      break;

    default: // should never occur, but...
      m_accelScale = 0.0;        // set a safe, though incorrect value
      throw std::logic_error(string(__FUNCTION__) + 
                             ": internal error, unsupported scale");
      break;
    }

  return true;
}

void BMA220::getAccelerometer(float *x, float *y, float *z)
{
  if (x)
    *x = m_accelX / m_accelScale;

  if (y)
    *y = m_accelY / m_accelScale;

  if (z)
    *z = m_accelZ / m_accelScale;
}

#ifdef JAVACALLBACK
float *BMA220::getAccelerometer()
{
  float *v = new float[3];
  getAccelerometer(&v[0], &v[1], &v[2]);
  return v;
}
#endif

uint8_t BMA220::getChipID()
{
  return readReg(REG_CHIPID);
}

uint8_t BMA220::getChipRevision()
{
  return readReg(REG_REVISIONID);
}

bool BMA220::setFilterConfig(FILTER_CONFIG_T filter)
{
  uint8_t reg = readReg(REG_FILTER_CONFIG);

  reg &= ~(_FILTER_CONFIG_FILTER_MASK << _FILTER_CONFIG_FILTER_SHIFT);

  reg |= (filter << _FILTER_CONFIG_FILTER_SHIFT);
  
  return writeReg(REG_FILTER_CONFIG, reg);
}

bool BMA220::setSerialHighBW(bool high)
{
  uint8_t reg = readReg(REG_FILTER_CONFIG);

  if (high)
    reg |= FILTER_CONFIG_SERIAL_HIGH_BW;
  else
    reg &= ~FILTER_CONFIG_SERIAL_HIGH_BW;
  
  return writeReg(REG_FILTER_CONFIG, reg);
}

bool BMA220::enableAxes(bool xEn, bool yEn, bool zEn)
{
  uint8_t reg = readReg(REG_ENABLE_CONFIG3);

  if (xEn)
    reg |= ENABLE_CONFIG3_X_CHAN;
  else
    reg &= ~ENABLE_CONFIG3_X_CHAN;

  if (yEn)
    reg |= ENABLE_CONFIG3_Y_CHAN;
  else
    reg &= ~ENABLE_CONFIG3_Y_CHAN;

  if (zEn)
    reg |= ENABLE_CONFIG3_Z_CHAN;
  else
    reg &= ~ENABLE_CONFIG3_Z_CHAN;

  return writeReg(REG_ENABLE_CONFIG3, reg);
}

uint8_t BMA220::suspend()
{
  return readReg(REG_SUSPEND);
}

uint8_t BMA220::softReset()
{
  return readReg(REG_SOFTRESET);
}

bool BMA220::sleep(bool enable)
{
  uint8_t reg = readReg(REG_ENABLE_CONFIG3);

  if (enable)
    reg |= ENABLE_CONFIG3_SLEEP_EN;
  else
    reg &= ~ENABLE_CONFIG3_SLEEP_EN;

  return writeReg(REG_ENABLE_CONFIG3, reg);
}

bool BMA220::setSleepDuration(SLEEP_DUR_T dur)
{
  uint8_t reg = readReg(REG_ENABLE_CONFIG3);

  reg &= ~(_ENABLE_CONFIG3_SLEEP_DUR_MASK << _ENABLE_CONFIG3_SLEEP_DUR_SHIFT);

  reg |= (dur << _ENABLE_CONFIG3_SLEEP_DUR_SHIFT);
  
  return writeReg(REG_ENABLE_CONFIG3, reg);
}

bool BMA220::setLowGThreshold(uint8_t thresh)
{
  uint8_t reg = readReg(REG_THRESHOLD);

  thresh &= 0x0f;

  reg &= ~(_THRESHOLD_LOW_MASK << _THRESHOLD_LOW_SHIFT);

  reg |= (thresh << _THRESHOLD_LOW_SHIFT);
  
  return writeReg(REG_THRESHOLD, reg);
}

bool BMA220::setHighGThreshold(uint8_t thresh)
{
  uint8_t reg = readReg(REG_THRESHOLD);

  thresh &= 0x0f;

  reg &= ~(_THRESHOLD_HIGH_MASK << _THRESHOLD_HIGH_SHIFT);

  reg |= (thresh << _THRESHOLD_HIGH_SHIFT);
  
  return writeReg(REG_THRESHOLD, reg);
}

bool BMA220::setLowGHysteresis(uint8_t hyst)
{
  uint8_t reg = readReg(REG_L_HYST_DUR);

  hyst &= _L_HYST_DUR_LOW_HY_MASK;

  reg &= ~(_L_HYST_DUR_LOW_HY_MASK << _L_HYST_DUR_LOW_HY_SHIFT);

  reg |= (hyst << _L_HYST_DUR_LOW_HY_SHIFT);
  
  return writeReg(REG_L_HYST_DUR, reg);
}

bool BMA220::setLowGDuration(uint8_t dur)
{
  uint8_t reg = readReg(REG_L_HYST_DUR);

  dur &= _L_HYST_DUR_LOW_DUR_MASK;

  reg &= ~(_L_HYST_DUR_LOW_DUR_MASK << _L_HYST_DUR_LOW_DUR_SHIFT);

  reg |= (dur << _L_HYST_DUR_LOW_DUR_SHIFT);
  
  return writeReg(REG_L_HYST_DUR, reg);
}

bool BMA220::setHighGHysteresis(uint8_t hyst)
{
  uint8_t reg = readReg(REG_H_HYST_DUR);

  hyst &= _H_HYST_DUR_HIGH_HY_MASK;

  reg &= ~(_H_HYST_DUR_HIGH_HY_MASK << _H_HYST_DUR_HIGH_HY_SHIFT);

  reg |= (hyst << _H_HYST_DUR_HIGH_HY_SHIFT);
  
  return writeReg(REG_H_HYST_DUR, reg);
}

bool BMA220::setHighGDuration(uint8_t dur)
{
  uint8_t reg = readReg(REG_H_HYST_DUR);

  dur &= _H_HYST_DUR_HIGH_DUR_MASK;

  reg &= ~(_H_HYST_DUR_HIGH_DUR_MASK << _H_HYST_DUR_HIGH_DUR_SHIFT);

  reg |= (dur << _H_HYST_DUR_HIGH_DUR_SHIFT);
  
  return writeReg(REG_H_HYST_DUR, reg);
}

bool BMA220::setTapDuration(uint8_t dur)
{
  uint8_t reg = readReg(REG_TAP_CONFIG);

  dur &= _TAP_CONFIG_DUR_MASK;

  reg &= ~(_TAP_CONFIG_DUR_MASK << _TAP_CONFIG_DUR_SHIFT);

  reg |= (dur << _TAP_CONFIG_DUR_SHIFT);
  
  return writeReg(REG_TAP_CONFIG, reg);
}

bool BMA220::setTapThreshold(uint8_t thresh)
{
  uint8_t reg = readReg(REG_TAP_CONFIG);

  thresh &= _TAP_CONFIG_THRESH_MASK;

  reg &= ~(_TAP_CONFIG_THRESH_MASK << _TAP_CONFIG_THRESH_SHIFT);

  reg |= (thresh << _TAP_CONFIG_THRESH_SHIFT);
  
  return writeReg(REG_TAP_CONFIG, reg);
}

bool BMA220::enableTapFilter(bool filt)
{
  uint8_t reg = readReg(REG_TAP_CONFIG);

  if (filt)
    reg |= TAP_CONFIG_FILTER;
  else
    reg &= ~TAP_CONFIG_FILTER;

  return writeReg(REG_TAP_CONFIG, reg);
}

bool BMA220::setSlopeDuration(uint8_t dur)
{
  uint8_t reg = readReg(REG_SLOPE_CONFIG);

  dur &= _SLOPE_CONFIG_DUR_MASK;

  reg &= ~(_SLOPE_CONFIG_DUR_MASK << _SLOPE_CONFIG_DUR_SHIFT);

  reg |= (dur << _SLOPE_CONFIG_DUR_SHIFT);
  
  return writeReg(REG_SLOPE_CONFIG, reg);
}

bool BMA220::setSlopeThreshold(uint8_t thresh)
{
  uint8_t reg = readReg(REG_SLOPE_CONFIG);

  thresh &= _SLOPE_CONFIG_THRESH_MASK;

  reg &= ~(_SLOPE_CONFIG_THRESH_MASK << _SLOPE_CONFIG_THRESH_SHIFT);

  reg |= (thresh << _SLOPE_CONFIG_THRESH_SHIFT);
  
  return writeReg(REG_SLOPE_CONFIG, reg);
}

bool BMA220::enableSlopeFilter(bool filt)
{
  uint8_t reg = readReg(REG_SLOPE_CONFIG);

  if (filt)
    reg |= SLOPE_CONFIG_FILTER;
  else
    reg &= ~SLOPE_CONFIG_FILTER;

  return writeReg(REG_SLOPE_CONFIG, reg);
}

uint8_t BMA220::getInterruptStatus1()
{
  return (readReg(REG_INT_STATUS1) & 0x8f);
}

BMA220::CONFIG_ORIENT_T BMA220::getOrient()
{
  uint8_t reg = readReg(REG_INT_STATUS2);

  reg &= (_INT_STATUS1_ORIENT_MASK << _INT_STATUS1_ORIENT_SHIFT);

  reg >>= _INT_STATUS1_ORIENT_SHIFT;

  return (CONFIG_ORIENT_T)reg;
}

uint8_t BMA220::getInterruptStatus2()
{
  return (readReg(REG_INT_STATUS2) & 0x1f);
}

bool BMA220::setInterruptEnables1(uint8_t bits)
{
  return writeReg(REG_ENABLE_CONFIG, bits);
}

uint8_t BMA220::getInterruptEnables1()
{
  return readReg(REG_ENABLE_CONFIG);
}

bool BMA220::setInterruptEnables2(uint8_t bits)
{
  uint8_t reg = readReg(REG_ENABLE_CONFIG2);

  // only the first 4 bits...
  bits &= 0x0f;
  reg &= 0x0f;

  reg |= bits;

  return writeReg(REG_ENABLE_CONFIG2, reg);
}

uint8_t BMA220::getInterruptEnables2()
{
  return (readReg(REG_ENABLE_CONFIG2) & 0x0f);
}

bool BMA220::setInterruptLatch(CONFIG2_LAT_T lat)
{
  uint8_t reg = readReg(REG_ENABLE_CONFIG2);

  reg &= ~(_ENABLE_CONFIG2_LAT_INT_MASK << _ENABLE_CONFIG2_LAT_INT_SHIFT);

  reg |= (lat << _ENABLE_CONFIG2_LAT_INT_SHIFT);
  
  return writeReg(REG_ENABLE_CONFIG2, reg);
}

bool BMA220::resetInterrupts()
{
  // This resets the interrupt controller, and should be called
  // whenever the interrupt configuration changes
  uint8_t reg = readReg(REG_ENABLE_CONFIG2);

  reg |= ENABLE_CONFIG2_RESET_INT;

  return writeReg(REG_ENABLE_CONFIG2, reg);
}

#ifdef SWIGJAVA
void BMA220::installISR(int gpio, mraa::Edge level,
                        IsrCallback *cb)
{
  installISR(gpio, level, generic_callback_isr, cb);
}
#endif

void BMA220::installISR(int gpio, mraa::Edge level, 
                        void (*isr)(void *), void *arg)
{
  // delete any existing ISR and GPIO context
  uninstallISR();

  // create gpio context
  m_gpioIntr = new mraa::Gpio(gpio);

  m_gpioIntr->dir(mraa::DIR_IN);
  m_gpioIntr->isr(level, isr, arg);
}

void BMA220::uninstallISR()
{
  if (m_gpioIntr)
    {
      m_gpioIntr->isrExit();
      delete m_gpioIntr;
      
      m_gpioIntr = 0;
    }
}

