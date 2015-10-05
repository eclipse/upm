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
#include <string>
#include <stdexcept>

#include "hp20x.h"

using namespace upm;
using namespace std;


HP20X::HP20X(int bus, uint8_t address):
  m_i2c(bus)
{
  m_addr = address;

  mraa::Result rv;
  if ( (rv = m_i2c.address(m_addr)) != mraa::SUCCESS)
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": I2c.address() failed");
      return;
    }
}

HP20X::~HP20X()
{
}

bool HP20X::init(DSR_BITS_T dsr)
{
  // wait for the device to report ready
  waitforDeviceReady();

  m_dsr = dsr;

  // enable compensation?  Datasheet says yes, but a register readback
  // says no.  Data does seem stable, so....
  compensationEnable(true);

  return true;
}

bool HP20X::isReady()
{
  uint8_t intsrc = readReg(REG_INT_SRC);
  
  if (intsrc & INT_SRC_DEV_RDY)
    return true;

  return false;
}

bool HP20X::waitforDeviceReady()
{
  const int maxRetries = 20;

  int retries = 0;

  while (retries < maxRetries)
    {
      if (isReady())
        return true;

      usleep(20000);
      retries++;
    }
  
  throw std::runtime_error(std::string(__FUNCTION__) +
                           ": timeout waiting for device to become ready");

  return false;
}

bool HP20X::writeCmd(uint8_t cmd)
{
  mraa::Result rv;
  if ((rv = m_i2c.writeByte(cmd)) != mraa::SUCCESS)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.writeByte() failed");
      return false;
    } 

  return true;
}

bool HP20X::writeReg(HP20X_REG_T reg, uint8_t data)
{
  waitforDeviceReady();

  uint8_t r = CMD_WRITE_REG | reg;

  mraa::Result rv;
  if ((rv = m_i2c.writeReg(r, data)) != mraa::SUCCESS)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.writeReg() failed");
      return false;
    }
 
  return true;
}

uint8_t HP20X::readReg(HP20X_REG_T reg)
{
  uint8_t r = CMD_READ_REG | reg;

  return m_i2c.readReg(r);
}

int HP20X::readData()
{
  uint8_t buf[3] = {0};

  if (!m_i2c.read(buf, 3))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.read() failed");
      return 0;
    }

  // handle 24bit sign extension
  int minus = 1;
  if (buf[0] & 0x80)
    {
      // negative
      buf[0] &= 0x3f;
      minus = -1;
    }

  return ( minus * ((buf[0] << 16) | (buf[1] << 8) | buf[2]) );
}

float HP20X::getTemperature()
{
  // wait for the device to report ready
  waitforDeviceReady();

  // start conversion, T only
  uint8_t cmd = CMD_ADC_CVT | (CHNL_T << CHNL_SHIFT) | (m_dsr << DSR_SHIFT);
  writeCmd(cmd);

  // wait for the device to report ready
  waitforDeviceReady();

  // now read the temperature
  writeCmd(CMD_READ_T);

  return ((float)readData() / 100.0);
}

float HP20X::getPressure()
{
  // wait for the device to report ready
  waitforDeviceReady();

  // start conversion, PT only
  uint8_t cmd = CMD_ADC_CVT | (CHNL_PT << CHNL_SHIFT) | (m_dsr << DSR_SHIFT);
  writeCmd(cmd);

  // wait for the device to report ready
  waitforDeviceReady();

  // now read the pressure
  writeCmd(CMD_READ_P);

  return ((float)readData() / 100.0);
}

float HP20X::getAltitude()
{
  // wait for the device to report ready
  waitforDeviceReady();

  // start conversion, PT only
  uint8_t cmd = CMD_ADC_CVT | (CHNL_PT << CHNL_SHIFT) | (m_dsr << DSR_SHIFT);
  writeCmd(cmd);

  // wait for the device to report ready
  waitforDeviceReady();

  // now read the pressure
  writeCmd(CMD_READ_A);

  return ((float)readData() / 100.0);
}

void HP20X::compensationEnable(bool enable)
{
  if (enable)
    writeReg(REG_PARA, PARA_CMPS_EN);
  else
    writeReg(REG_PARA, 0);
}

bool HP20X::setInterruptEnable(uint8_t bits)
{
  return writeReg(REG_INT_EN, bits);
}

bool HP20X::setInterruptConfig(uint8_t bits)
{
  return writeReg(REG_INT_CFG, bits);
}

uint8_t HP20X::getInterruptSource()
{
  return readReg(REG_INT_SRC);
}

void HP20X::setDSR(DSR_BITS_T dsr)
{
  m_dsr = dsr;
}

void HP20X::recalibrateInternal()
{
  waitforDeviceReady();
  writeCmd(CMD_ANA_CAL);
}

void HP20X::softReset()
{
  waitforDeviceReady();
  writeCmd(CMD_SOFT_RST);
  waitforDeviceReady();
}

void HP20X::setAltitudeOffset(int16_t off)
{
  writeReg(REG_ALT_OFF_LSB, (off & 0xff));
  writeReg(REG_ALT_OFF_MSB, ((off >> 8) & 0xff));
}

void HP20X::setPAThreshholds(int16_t low, int16_t med, int16_t high)
{
  // low
  writeReg(REG_PA_L_TH_LSB, (low & 0xff));
  writeReg(REG_PA_L_TH_MSB, ((low >> 8) & 0xff));

  // medium
  writeReg(REG_PA_M_TH_LSB, (med & 0xff));
  writeReg(REG_PA_M_TH_MSB, ((med >> 8) & 0xff));

  // high
  writeReg(REG_PA_H_TH_LSB, (high & 0xff));
  writeReg(REG_PA_H_TH_MSB, ((high >> 8) & 0xff));
}

void HP20X::setTemperatureThreshholds(int8_t low, int8_t med, int8_t high)
{
  // low
  writeReg(REG_T_L_TH, low);

  // medium
  writeReg(REG_T_M_TH, med);

  // high
  writeReg(REG_T_H_TH, high);
}

