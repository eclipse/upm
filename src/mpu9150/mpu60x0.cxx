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
#include <string.h>

#include "mpu60x0.h"

using namespace upm;
using namespace std;


MPU60X0::MPU60X0(int bus, uint8_t address) :
  m_i2c(bus), m_gpioIRQ(0)
{
  m_addr = address;

  m_accelX = 0.0;
  m_accelY = 0.0;
  m_accelZ = 0.0;
  
  m_gyroX = 0.0;
  m_gyroY = 0.0;
  m_gyroZ = 0.0;
  
  m_temp = 0.0;

  m_accelScale = 1.0;
  m_gyroScale = 1.0;

  mraa::Result rv;
  if ( (rv = m_i2c.address(m_addr)) != mraa::SUCCESS)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.address() failed");
      return;
    }
}

MPU60X0::~MPU60X0()
{
  uninstallISR();
}

bool MPU60X0::init()
{
  // first, take the device out of sleep mode
  if (!setSleep(false))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": Unable to wake up device");
      return false;
    }

  // set the clock source to use the gyroscope PLL rather than the
  // internal clock for stability
  if (!setClockSource(PLL_XG))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": Unable to set clock source");
      return false;
    }

  usleep(5000);

  // enable temperature measurement (default on power up, but let's be sure)
  enableTemperatureSensor(true);

  // set the gyro and accel scale bits to reasonable values
  setGyroscopeScale(FS_500);
  setAccelerometerScale(AFS_2);

  // enable the DLPF
  setDigitalLowPassFilter(DLPF_94_98);

  // let things stabilize...
  usleep(100000);

  return true;
}


void MPU60X0::update()
{
  // read all of the sensor registers - accel, gyro, and temp
  uint8_t buffer[14];

  memset(buffer, 0, 14);
  readRegs(REG_ACCEL_XOUT_H, buffer, 14);

  int16_t ax, ay, az;
  int16_t temp;
  int16_t gx, gy, gz;

  ax =  ( (buffer[0] << 8) | buffer[1] );
  ay =  ( (buffer[2] << 8) | buffer[3] );
  az =  ( (buffer[4] << 8) | buffer[5] );

  temp = ( (buffer[6] << 8) | buffer[7] );

  gx =  ( (buffer[8] << 8) | buffer[9] );
  gy =  ( (buffer[10] << 8) | buffer[11] );
  gz =  ( (buffer[12] << 8) | buffer[13] );

  // now stash them
  m_accelX = float(ax);
  m_accelY = float(ay);
  m_accelZ = float(az);

  m_temp = float(temp);

  m_gyroX = float(gx);
  m_gyroY = float(gy);
  m_gyroZ = float(gz);
}

uint8_t MPU60X0::readReg(uint8_t reg)
{
  return m_i2c.readReg(reg);
}

void MPU60X0::readRegs(uint8_t reg, uint8_t *buffer, int len)
{
  m_i2c.readBytesReg(reg, buffer, len);
}

bool MPU60X0::writeReg(uint8_t reg, uint8_t val)
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

bool MPU60X0::setSleep(bool enable)
{
  uint8_t reg = readReg(REG_PWR_MGMT_1);

  if (enable)
    reg |= PWR_SLEEP;
  else
    reg &= ~PWR_SLEEP;

  return writeReg(REG_PWR_MGMT_1, reg);
}

bool MPU60X0::setClockSource(CLKSEL_T clk)
{
  uint8_t reg = readReg(REG_PWR_MGMT_1);

  reg &= ~(_CLKSEL_MASK << _CLKSEL_SHIFT);

  reg |= (clk << _CLKSEL_SHIFT);

  return writeReg(REG_PWR_MGMT_1, reg);
}

bool MPU60X0::setGyroscopeScale(FS_SEL_T scale)
{
  uint8_t reg = readReg(REG_GYRO_CONFIG);

  reg &= ~(_FS_SEL_MASK << _FS_SEL_SHIFT);

  reg |= (scale << _FS_SEL_SHIFT);

  if (!writeReg(REG_GYRO_CONFIG, reg))
    {
      return false;
    }

  // store scaling factor

  switch (scale)
    {
    case FS_250:
      m_gyroScale = 131.0;
      break;

    case FS_500:
      m_gyroScale = 65.5;
      break;

    case FS_1000:
      m_gyroScale = 32.8;
      break;

    case FS_2000:
      m_gyroScale = 16.4;
      break;

    default: // should never occur, but...
      m_gyroScale = 1.0;        // set a safe, though incorrect value
      throw std::logic_error(string(__FUNCTION__) + 
                             ": internal error, unsupported scale");
      break;
    }

  return true;
}

bool MPU60X0::setAccelerometerScale(AFS_SEL_T scale)
{
  uint8_t reg = readReg(REG_ACCEL_CONFIG);

  reg &= ~(_AFS_SEL_MASK << _AFS_SEL_SHIFT);

  reg |= (scale << _AFS_SEL_SHIFT);

  if (!writeReg(REG_ACCEL_CONFIG, reg))
    {
      return false;
    }

  // store scaling factor
  
  switch (scale)
    {
    case AFS_2:
      m_accelScale = 16384.0;
      break;

    case AFS_4:
      m_accelScale = 8192.0;
      break;

    case AFS_8:
      m_accelScale = 4096.0;
      break;

    case AFS_16:
      m_accelScale = 2048.0;
      break;

    default: // should never occur, but...
      m_accelScale = 1.0;        // set a safe, though incorrect value
      throw std::logic_error(string(__FUNCTION__) + 
                             ": internal error, unsupported scale");
      break;
    }

  return true;
}

bool MPU60X0::setDigitalLowPassFilter(DLPF_CFG_T dlp)
{
  uint8_t reg = readReg(REG_CONFIG);

  reg &= ~(_CONFIG_DLPF_MASK << _CONFIG_DLPF_SHIFT);

  reg |= (dlp << _CONFIG_DLPF_SHIFT);

  return writeReg(REG_CONFIG, reg);
}

bool MPU60X0::setSampleRateDivider(uint8_t div)
{
  return writeReg(REG_SMPLRT_DIV, div);
}

uint8_t MPU60X0::getSampleRateDivider()
{
  return readReg(REG_SMPLRT_DIV);
}

void MPU60X0::getAccelerometer(float *x, float *y, float *z)
{
  if (x)
    *x = m_accelX / m_accelScale;

  if (y)
    *y = m_accelY / m_accelScale;

  if (z)
    *z = m_accelZ / m_accelScale;
}

void MPU60X0::getGyroscope(float *x, float *y, float *z)
{
  if (x)
    *x = m_gyroX / m_gyroScale;

  if (y)
    *y = m_gyroY / m_gyroScale;

  if (z)
    *z = m_gyroZ / m_gyroScale;
}

float MPU60X0::getTemperature()
{
  // this equation is taken from the datasheet
  return (m_temp / 340.0) + 36.53;
}

bool MPU60X0::enableTemperatureSensor(bool enable)
{
  uint8_t reg = readReg(REG_PWR_MGMT_1);

  if (enable)
    reg &= ~TEMP_DIS;
  else
    reg |= TEMP_DIS;

  return writeReg(REG_PWR_MGMT_1, reg);
}

bool MPU60X0::setExternalSync(EXT_SYNC_SET_T val)
{
  uint8_t reg = readReg(REG_CONFIG);

  reg &= ~(_CONFIG_EXT_SYNC_SET_MASK << _CONFIG_EXT_SYNC_SET_SHIFT);

  reg |= (val << _CONFIG_EXT_SYNC_SET_SHIFT);

  return writeReg(REG_CONFIG, reg);
}

bool MPU60X0::enableI2CBypass(bool enable)
{
  uint8_t reg = readReg(REG_INT_PIN_CFG);

  if (enable)
    reg |= I2C_BYPASS_ENABLE;
  else
    reg &= ~I2C_BYPASS_ENABLE;

  return writeReg(REG_INT_PIN_CFG, reg);
}

bool MPU60X0::setMotionDetectionThreshold(uint8_t thr)
{
  return writeReg(REG_MOT_THR, thr);
}

uint8_t MPU60X0::getInterruptStatus()
{
  return readReg(REG_INT_STATUS);
}

bool MPU60X0::setInterruptEnables(uint8_t enables)
{
  return writeReg(REG_INT_ENABLE, enables);
}

uint8_t MPU60X0::getInterruptEnables()
{
  return readReg(REG_INT_ENABLE);
}

bool MPU60X0::setInterruptPinConfig(uint8_t cfg)
{
  return writeReg(REG_INT_PIN_CFG, cfg);
}

uint8_t MPU60X0::getInterruptPinConfig()
{
  return readReg(REG_INT_PIN_CFG);
}

#ifdef JAVACALLBACK
void MPU60X0::installISR(int gpio, mraa::Edge level,
                         IsrCallback *cb)
{
        installISR(gpio, level, generic_callback_isr, cb);
}
#endif

void MPU60X0::installISR(int gpio, mraa::Edge level, 
                         void (*isr)(void *), void *arg)
{
  // delete any existing ISR and GPIO context
  uninstallISR();

  // greate gpio context
  m_gpioIRQ = new mraa::Gpio(gpio);

  m_gpioIRQ->dir(mraa::DIR_IN);
  m_gpioIRQ->isr(level, isr, arg);
}

void MPU60X0::uninstallISR()
{
  if (m_gpioIRQ)
    {
      m_gpioIRQ->isrExit();
      delete m_gpioIRQ;
      
      m_gpioIRQ = 0;
    }
}
