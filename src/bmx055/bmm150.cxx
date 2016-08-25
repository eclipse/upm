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

// The trimming algorithms are taken from the Bosch BMM050 driver code

/****************************************************************************
* Copyright (C) 2015 - 2016 Bosch Sensortec GmbH
*
* File : bmm050.h
*
* Date : 2016/03/17
*
* Revision : 2.0.5 $
*
* Usage: Sensor Driver for BMM050 and BMM150 sensor
*
****************************************************************************
*
* section License
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
*   Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
*
*   Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
*
*   Neither the name of the copyright holder nor the names of the
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER
* OR CONTRIBUTORS BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
* OR CONSEQUENTIAL DAMAGES(INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
*
* The information provided is believed to be accurate and reliable.
* The copyright holder assumes no responsibility
* for the consequences of use
* of such information nor for any infringement of patents or
* other rights of third parties which may result from its use.
* No license is granted by implication or otherwise under any patent or
* patent rights of the copyright holder.
**************************************************************************/


#include <unistd.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string.h>

#include "bmm150.hpp"

#define BMM150_DEFAULT_CHIPID 0x32

using namespace upm;
using namespace std;

BMM150::BMM150(int bus, int addr, int cs) :
  m_i2c(0), m_spi(0), m_gpioIntr(0), m_gpioDR(0), m_gpioCS(0)
{
  m_magX = 0;
  m_magY = 0;
  m_magZ = 0;

  m_hall = 0;

  m_dig_x1 = 0;
  m_dig_y1 = 0;

  m_dig_z4 = 0;
  m_dig_x2 = 0;
  m_dig_y2 = 0;

  m_dig_z2 = 0;
  m_dig_z1 = 0;
  m_dig_xyz1 = 0;
  m_dig_z3 = 0;
  m_dig_xy2 = 0;
  m_dig_xy1 = 0;

  if (addr < 0)
    {
      m_addr = 0;
      m_isSPI = true;
    }
  else
    {
      m_addr = uint8_t(addr);
      m_isSPI = false;
    }


  if (m_isSPI)
    {
      m_spi = new mraa::Spi(bus);

      // Only create cs context if we are actually using a valid pin.
      // A hardware controlled pin should specify cs as -1.
      if (cs >= 0)
        {
          m_gpioCS = new mraa::Gpio(cs);
          m_gpioCS->dir(mraa::DIR_OUT);
        }

      m_spi->mode(mraa::SPI_MODE0);
      m_spi->frequency(5000000);
    }
  else
    {
      // I2C
      m_i2c = new mraa::I2c(bus);

      mraa::Result rv;
      if ((rv = m_i2c->address(m_addr)) != mraa::SUCCESS)
        {
          throw std::runtime_error(string(__FUNCTION__) +
                                   ": I2c.address() failed");
        }
    }

  // power bit must be on for chip ID to be accessable
  setPowerBit(true);
  m_opmode = OPERATION_MODE_SLEEP;

  usleep(50000);

  // check the chip id
  uint8_t chipID = getChipID();
  if (chipID != BMM150_DEFAULT_CHIPID)
    {
      throw std::runtime_error(string(__FUNCTION__)
                               + ": invalid chip ID.  Expected "
                               + std::to_string(int(BMM150_DEFAULT_CHIPID))
                               + ", got "
                               + std::to_string(int(chipID)));
    }

  // get trim data
  readTrimData();

  // call init with default options
  init();
}

BMM150::~BMM150()
{
  uninstallISR(INTERRUPT_INT);
  uninstallISR(INTERRUPT_DR);

  if (m_i2c)
    delete m_i2c;
  if (m_spi)
    delete m_spi;
  if(m_gpioCS)
    delete m_gpioCS;
}

void BMM150::init(USAGE_PRESETS_T usage)
{
  setPowerBit(true);
  setOpmode(OPERATION_MODE_NORMAL);

  usleep(50000); // 50ms, in case we are waking up

  setPresetMode(usage);

  // settle
  usleep(50000);
}

void BMM150::update()
{
  // special care when in a forced mode - need to trigger a
  // measurement, and wait for the opmode to return to OPMODE_SLEEP,
  // then we can read the values.

  if (m_opmode == OPERATION_MODE_FORCED)
    {
      // trigger measurement
      setOpmode(OPERATION_MODE_FORCED);

      // opmode will return to sleep after measurement is complete
      do {
        usleep(5000);
      } while (getOpmode() == OPERATION_MODE_FORCED);
    }

  const int bufLen = 8;
  uint8_t buf[bufLen];

  if (readRegs(REG_MAG_X_LSB, buf, bufLen) != bufLen)
    {
      throw std::runtime_error(string(__FUNCTION__)
                               + ": readRegs() failed to read "
                               + std::to_string(bufLen)
                               + " bytes");
    }

  // we need to get the hall data first, since it's needed for the
  // bosch compensation functions for each of the xyz axes

  m_hall = uint16_t(buf[7] << 8 | (buf[6] &
                                   (_MAG_RHALL_LSB_LSB_MASK <<
                                    _MAG_RHALL_LSB_LSB_SHIFT)));
  m_hall /= 4;

  int16_t val;

  // x
  val = int16_t(buf[1] << 8 | (buf[0] & (_MAG_XY_LSB_LSB_MASK <<
                                         _MAG_XY_LSB_LSB_SHIFT)));
  val /= 8;
  m_magX = bmm050_compensate_X_float(val, m_hall);

  // y
  val = int16_t(buf[3] << 8 | (buf[2] & (_MAG_XY_LSB_LSB_MASK <<
                                         _MAG_XY_LSB_LSB_SHIFT)));
  val /= 8;
  m_magY = bmm050_compensate_Y_float(val, m_hall);

  // z
  val = int16_t(buf[5] << 8 | (buf[4] & (_MAG_Z_LSB_LSB_MASK <<
                                         _MAG_Z_LSB_LSB_SHIFT)));
  val /= 2;
  m_magZ = bmm050_compensate_Z_float(val, m_hall);
}

uint8_t BMM150::readReg(uint8_t reg)
{
  if (m_isSPI)
    {
      reg |= 0x80; // needed for read
      uint8_t pkt[2] = {reg, 0};

      csOn();
      if (m_spi->transfer(pkt, pkt, 2))
        {
          csOff();
          throw std::runtime_error(string(__FUNCTION__)
                                   + ": Spi.transfer() failed");
        }
      csOff();

      return pkt[1];
    }
  else
    return m_i2c->readReg(reg);
}

int BMM150::readRegs(uint8_t reg, uint8_t *buffer, int len)
{
  if (m_isSPI)
    {
      reg |= 0x80; // needed for read

      uint8_t sbuf[len + 1];
      memset((char *)sbuf, 0, len + 1);
      sbuf[0] = reg;

      // We need to do it this way for edison - ie: use a single
      // transfer rather than breaking it up into two like we used to.
      // This means a buffer copy is now required, but that's the way
      // it goes.

      csOn();
      if (m_spi->transfer(sbuf, sbuf, len + 1))
        {
          csOff();
          throw std::runtime_error(string(__FUNCTION__)
                                   + ": Spi.transfer(buf) failed");
        }
      csOff();

      // now copy it into user buffer
      for (int i=0; i<len; i++)
        buffer[i] = sbuf[i + 1];

      return len;
    }
  else
    return m_i2c->readBytesReg(reg, buffer, len);
}

void BMM150::writeReg(uint8_t reg, uint8_t val)
{
  if (m_isSPI)
    {
      reg &= 0x7f; // mask off 0x80 for writing
      uint8_t pkt[2] = {reg, val};

      csOn();
      if (m_spi->transfer(pkt, NULL, 2))
        {
          csOff();
          throw std::runtime_error(string(__FUNCTION__)
                                   + ": Spi.transfer() failed");
        }
      csOff();
    }
  else
    {

      mraa::Result rv;
      if ((rv = m_i2c->writeReg(reg, val)) != mraa::SUCCESS)
        {
          throw std::runtime_error(std::string(__FUNCTION__)
                                   + ": I2c.writeReg() failed");
        }
    }
}

void BMM150::csOn()
{
  if (m_gpioCS)
    m_gpioCS->write(0);
}

void BMM150::csOff()
{
  if (m_gpioCS)
    m_gpioCS->write(1);
}

uint8_t BMM150::getChipID()
{
  return readReg(REG_CHIP_ID);
}

void BMM150::getMagnetometer(float *x, float *y, float *z)
{
  if (x)
    *x = m_magX;

  if (y)
    *y = m_magY;

  if (z)
    *z = m_magZ;
}

float *BMM150::getMagnetometer()
{
  static float v[3];

  getMagnetometer(&v[0], &v[1], &v[2]);
  return v;
}

void BMM150::reset()
{
  // mask off reserved bits
  uint8_t reg = readReg(REG_POWER_CTRL) & ~_POWER_CTRL_RESERVED_BITS;

  reg |= POWER_CTRL_SOFT_RESET0 | POWER_CTRL_SOFT_RESET1;

  writeReg(REG_POWER_CTRL, reg);
  sleep(1);
  // device will return to SLEEP mode...
}

void BMM150::setOutputDataRate(DATA_RATE_T odr)
{
  uint8_t reg = readReg(REG_OPMODE);

  reg &= ~(_OPMODE_DATA_RATE_MASK << _OPMODE_DATA_RATE_SHIFT);
  reg |= (odr << _OPMODE_DATA_RATE_SHIFT);

  writeReg(REG_OPMODE, reg);
}

void BMM150::setPowerBit(bool power)
{
  // mask off reserved bits
  uint8_t reg = readReg(REG_POWER_CTRL) & ~_POWER_CTRL_RESERVED_BITS;

  if (power)
    reg |= POWER_CTRL_POWER_CTRL_BIT;
  else
    reg &= ~POWER_CTRL_POWER_CTRL_BIT;

  writeReg(REG_POWER_CTRL, reg);
}

void BMM150::setOpmode(OPERATION_MODE_T opmode)
{
  uint8_t reg = readReg(REG_OPMODE);

  reg &= ~(_OPMODE_OPERATION_MODE_MASK << _OPMODE_OPERATION_MODE_SHIFT);
  reg |= (opmode << _OPMODE_OPERATION_MODE_SHIFT);

  writeReg(REG_OPMODE, reg);
  m_opmode = opmode;
}

BMM150::OPERATION_MODE_T BMM150::getOpmode()
{
  uint8_t reg = readReg(REG_OPMODE);

  reg &= (_OPMODE_OPERATION_MODE_MASK << _OPMODE_OPERATION_MODE_SHIFT);
  reg >>=  _OPMODE_OPERATION_MODE_SHIFT;

  return static_cast<OPERATION_MODE_T>(reg);
}

uint8_t BMM150::getInterruptEnable()
{
  return readReg(REG_INT_EN);
}

void BMM150::setInterruptEnable(uint8_t bits)
{
  writeReg(REG_INT_EN, bits);
}

uint8_t BMM150::getInterruptConfig()
{
  return readReg(REG_INT_CONFIG);
}

void BMM150::setInterruptConfig(uint8_t bits)
{
  writeReg(REG_INT_CONFIG, bits);
}

uint8_t BMM150::getInterruptStatus()
{
  return readReg(REG_INT_STATUS);
}

void BMM150::readTrimData()
{
  int bufLen = 10;
  uint8_t calibData[bufLen];

  // 2 bytes first
  readRegs(REG_TRIM_DIG_X1, calibData, 2);

  m_dig_x1 = int8_t(calibData[0]);
  m_dig_y1 = int8_t(calibData[1]);

  // next block of 4 bytes
  readRegs(REG_TRIM_DIG_Z4_LSB, calibData, 4);

  m_dig_z4 = int16_t((calibData[1] << 8) | calibData[0]);
  m_dig_x2 = int8_t(calibData[2]);
  m_dig_y2 = int8_t(calibData[3]);

  // final block of 10 bytes
  readRegs(REG_TRIM_DIG_Z2_LSB, calibData, 10);

  m_dig_z2 = int16_t((calibData[1] << 8) | calibData[0]);
  m_dig_z1 = uint16_t((calibData[3] << 8) | calibData[2]);
  m_dig_xyz1 = uint16_t((calibData[5] << 8) | calibData[4]);
  m_dig_z3 = int16_t((calibData[7] << 8) | calibData[6]);
  m_dig_xy2 = int8_t(calibData[8]);
  m_dig_xy1 = calibData[9];
}

void BMM150::setRepetitionsXY(uint8_t reps)
{
  writeReg(REG_REP_XY, reps);
}

void BMM150::setRepetitionsZ(uint8_t reps)
{
  writeReg(REG_REP_Z, reps);
}

void BMM150::setPresetMode(USAGE_PRESETS_T usage)
{
  // these recommended presets come from the datasheet, Table 3,
  // Section 4.2
  switch (usage)
    {
    case USAGE_LOW_POWER:
      setRepetitionsXY(3);
      setRepetitionsZ(3);
      setOutputDataRate(DATA_RATE_10HZ);

      break;

    case USAGE_REGULAR:
      setRepetitionsXY(9);
      setRepetitionsZ(15);
      setOutputDataRate(DATA_RATE_10HZ);

      break;

    case USAGE_ENHANCED_REGULAR:
      setRepetitionsXY(15);
      setRepetitionsZ(27);
      setOutputDataRate(DATA_RATE_10HZ);

      break;

    case USAGE_HIGH_ACCURACY:
      setRepetitionsXY(47);
      setRepetitionsZ(83);
      setOutputDataRate(DATA_RATE_20HZ);

      break;

    default:
      throw std::out_of_range(string(__FUNCTION__) +
                              ": Invalid usage enum passed");
    }
}

#if defined(SWIGJAVA) || (JAVACALLBACK)
void BMM150::installISR(INTERRUPT_PINS_T intr, int gpio, mraa::Edge level,
                         jobject runnable)
{
  // delete any existing ISR and GPIO context
  uninstallISR(intr);

  // create gpio context
  getPin(intr) = new mraa::Gpio(gpio);

  getPin(intr)->dir(mraa::DIR_IN);
  getPin(intr)->isr(level, runnable);
}
#else
void BMM150::installISR(INTERRUPT_PINS_T intr, int gpio, mraa::Edge level,
                         void (*isr)(void *), void *arg)
{
  // delete any existing ISR and GPIO context
  uninstallISR(intr);

  // create gpio context
  getPin(intr) = new mraa::Gpio(gpio);

  getPin(intr)->dir(mraa::DIR_IN);
  getPin(intr)->isr(level, isr, arg);
}
#endif

void BMM150::uninstallISR(INTERRUPT_PINS_T intr)
{
  if (getPin(intr))
    {
      getPin(intr)->isrExit();
      delete getPin(intr);

      getPin(intr) = 0;
    }
}

mraa::Gpio*& BMM150::getPin(INTERRUPT_PINS_T intr)
{
  switch(intr)
    {
    case INTERRUPT_INT:
      return m_gpioIntr;
      break;

    case INTERRUPT_DR:
      return m_gpioDR;
      break;

    default:
      throw std::out_of_range(string(__FUNCTION__) +
                              ": Invalid interrupt enum passed");
    }
}

// Bosch compensation functions

float BMM150::bmm050_compensate_X_float(int16_t mag_data_x, uint16_t data_r)
{
  float inter_retval = 0;

  if (mag_data_x != -4096	/* no overflow */
      ) {
    if ((data_r != 0)
        && (m_dig_xyz1 != 0)) {
      inter_retval = ((((float)m_dig_xyz1)
                       * 16384.0 / data_r) - 16384.0);
    } else {
      inter_retval = 0.0f;
      return inter_retval;
    }
    inter_retval = (((mag_data_x * ((((((float)m_dig_xy2) *
                                       (inter_retval*inter_retval /
                                        268435456.0) +
                                       inter_retval * ((float)m_dig_xy1)
                                       / 16384.0)) + 256.0) *
                                    (((float)m_dig_x2) + 160.0)))
                     / 8192.0)
                    + (((float)m_dig_x1) *
                       8.0)) / 16.0;
  } else {
    inter_retval = 0.0f;
  }
  return inter_retval;
}

float BMM150::bmm050_compensate_Y_float(int16_t mag_data_y, uint16_t data_r)
{
  float inter_retval = 0;

  if (mag_data_y != -4096 /* no overflow */
      ) {
    if ((data_r != 0)
        && (m_dig_xyz1 != 0)) {
      inter_retval = ((((float)m_dig_xyz1)
                       * 16384.0
                       /data_r) - 16384.0);
    } else {
      inter_retval = 0.0f;
      return inter_retval;
    }
    inter_retval = (((mag_data_y * ((((((float)m_dig_xy2) *
                                       (inter_retval*inter_retval
                                        / 268435456.0) +
                                       inter_retval * ((float)m_dig_xy1)
                                       / 16384.0)) +
                                     256.0) *
                                    (((float)m_dig_y2) + 160.0)))
                     / 8192.0) +
                    (((float)m_dig_y1) * 8.0))
      / 16.0;
  } else {
    /* overflow, set output to 0.0f */
    inter_retval = 0.0f;
  }
  return inter_retval;
}

float BMM150::bmm050_compensate_Z_float(int16_t mag_data_z, uint16_t data_r)
{
  float inter_retval = 0;
  /* no overflow */
  if (mag_data_z != -16384) {
    if ((m_dig_z2 != 0)
        && (m_dig_z1 != 0)
        && (m_dig_xyz1 != 0)
        && (data_r != 0)) {
      inter_retval = ((((((float)mag_data_z)-
                         ((float)m_dig_z4)) * 131072.0)-
                       (((float)m_dig_z3)*(((float)data_r)
                                                   -((float)m_dig_xyz1))))
                      /((((float)m_dig_z2)+
                         ((float)m_dig_z1)*((float)data_r) /
                         32768.0) * 4.0)) / 16.0;
    }
  } else {
    /* overflow, set output to 0.0f */
    inter_retval = 0.0f;
  }
  return inter_retval;
}
