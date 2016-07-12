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
#include <string.h>

#include "bma250e.hpp"

using namespace upm;
using namespace std;

// conversion from celsius to fahrenheit

static float c2f(float c)
{
  return (c * (9.0 / 5.0) + 32.0);
}

BMA250E::BMA250E(int bus, int addr, int cs) :
  m_i2c(0), m_spi(0), m_gpioIntr1(0), m_gpioIntr2(0), m_gpioCS(0)
{
  m_accX = 0;
  m_accY = 0;
  m_accZ = 0;
  m_accScale = 0;
  m_temperature = 0.0;

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

  // check the chip id

  uint8_t chipID = getChipID();

  m_useFIFO = false;

  // check the various chips id's and set appropriate capabilities.
  // Bail if the chip id is unknown.
  switch (chipID)
    {
    case 0xf9: // standalone bma250e
      m_resolution = RESOLUTION_10BITS;
      m_fifoAvailable = true;

      break;

    case 0xfa: // bmx055, bmi055 variants, 12b resolution
      m_resolution = RESOLUTION_12BITS;
      m_fifoAvailable = true;

      break;

    case 0x03: // bmc050 variant, no FIFO, 12b resolution
      m_resolution = RESOLUTION_12BITS;
      m_fifoAvailable = false;

      break;

    default:
      throw std::runtime_error(string(__FUNCTION__)
                               + ": invalid chip ID.  Expected "
                               + "3, 249, or 250 "
                               + ", got "
                               + std::to_string(int(chipID)));
    }

  // call init with default options
  init();
}

BMA250E::~BMA250E()
{
  uninstallISR(INTERRUPT_INT1);
  uninstallISR(INTERRUPT_INT2);

  if (m_i2c)
    delete m_i2c;
  if (m_spi)
    delete m_spi;
  if(m_gpioCS)
    delete m_gpioCS;
}

void BMA250E::init(POWER_MODE_T pwr, RANGE_T range, BW_T bw)
{
  setPowerMode(pwr);
  usleep(50000); // 50ms, in case we are waking up

  // set our range and bandwidth
  setRange(range);
  setBandwidth(bw);

  // make sure register shadowing is enabled
  enableRegisterShadowing(true);

  // enable output filtering
  enableOutputFiltering(true);

  // use the FIFO by default, if we have one
  fifoConfig(FIFO_MODE_BYPASS, FIFO_DATA_SEL_XYZ);
  enableFIFO(true);

  // make sure low power mode LPM2 is enabled in case we go to low
  // power or suspend mode. LPM1 mode (the default) requires register
  // writes to be drastically slowed down when enabled, which we
  // cannot handle.
  setLowPowerMode2();

  // settle
  usleep(50000);
}

void BMA250E::update()
{
  int bufLen = 7; // max, non-FIFO
  uint8_t startReg = REG_ACCD_X_LSB;

  if (m_useFIFO)
    {
      bufLen = 6;
      startReg = REG_FIFO_DATA;
    }

  uint8_t buf[bufLen];

  if (readRegs(startReg, buf, bufLen) != bufLen)
    {
      throw std::runtime_error(string(__FUNCTION__)
                               + ": readRegs() failed to read "
                               + std::to_string(bufLen)
                               + " bytes");
    }

  uint8_t mask, shift;
  float divisor;

  switch (m_resolution)
    {
    case RESOLUTION_10BITS:
      mask = _ACCD10_LSB_MASK;
      shift = _ACCD10_LSB_SHIFT;
      divisor = 64.0;

      break;

    case RESOLUTION_12BITS:
      mask = _ACCD12_LSB_MASK;
      shift = _ACCD12_LSB_SHIFT;
      divisor = 16.0;

      break;

    default:
      throw std::logic_error(string(__FUNCTION__)
                             + ": internal error: invalid resolution "
                             + std::to_string(int(m_resolution)));
    }

  int16_t val;

  // x
  val = int16_t(buf[1] << 8 | (buf[0] & (mask << shift)));
  m_accX = float(val / divisor);

  // y
  val = int16_t(buf[3] << 8 | (buf[2] & (mask << shift)));
  m_accY = float(val / divisor);

  // z
  val = int16_t(buf[5] << 8 | (buf[4] & (mask << shift)));
  m_accZ = float(val / divisor);

  // get the temperature...

  uint8_t temp = 0;
  if (m_useFIFO)
    {
      // we have to read temperature separately...
      temp = readReg(REG_TEMP);
    }
  else
    {
      // we've already got it
      temp = buf[6];
    }

  // .5K/LSB, 23C center point
  m_temperature = (float(temp) / 2.0) + 23.0;
}

void BMA250E::enableFIFO(bool useFIFO)
{
  if (m_fifoAvailable)
    m_useFIFO = useFIFO;
}

uint8_t BMA250E::readReg(uint8_t reg)
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

int BMA250E::readRegs(uint8_t reg, uint8_t *buffer, int len)
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

void BMA250E::writeReg(uint8_t reg, uint8_t val)
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
      if (m_i2c->writeReg(reg, val) != mraa::SUCCESS)
        {
          throw std::runtime_error(std::string(__FUNCTION__)
                                   + ": I2c.writeReg() failed");
        }
    }
}

void BMA250E::csOn()
{
  if (m_gpioCS)
    m_gpioCS->write(0);
}

void BMA250E::csOff()
{
  if (m_gpioCS)
    m_gpioCS->write(1);
}

uint8_t BMA250E::getChipID()
{
  return readReg(REG_CHIP_ID);
}

void BMA250E::getAccelerometer(float *x, float *y, float *z)
{
  if (x)
    *x = (m_accX * m_accScale) / 1000.0;

  if (y)
    *y = (m_accY * m_accScale) / 1000.0;

  if (z)
    *z = (m_accZ * m_accScale) / 1000.0;
}

float *BMA250E::getAccelerometer()
{
  static float v[3];

  getAccelerometer(&v[0], &v[1], &v[2]);
  return v;
}

float BMA250E::getTemperature(bool fahrenheit)
{
  if (fahrenheit)
    return c2f(m_temperature);
  else
    return m_temperature;
}

void BMA250E::reset()
{
  writeReg(REG_SOFTRESET, BMA250E_RESET_BYTE);
  sleep(1);
}

void BMA250E::setRange(RANGE_T range)
{
  switch (m_resolution)
    {
    case RESOLUTION_10BITS:
      switch(range)
        {
        case RANGE_2G:
          m_accScale = 3.91; // milli-gravities
          break;

        case RANGE_4G:
          m_accScale = 7.81;
          break;

        case RANGE_8G:
          m_accScale = 15.63;
          break;

        case RANGE_16G:
          m_accScale = 31.25;
          break;
        }

      break;

    case RESOLUTION_12BITS:
      switch(range)
        {
        case RANGE_2G:
          m_accScale = 0.98; // milli-gravities
          break;

        case RANGE_4G:
          m_accScale = 1.95;
          break;

        case RANGE_8G:
          m_accScale = 3.91;
          break;

        case RANGE_16G:
          m_accScale = 7.81;
          break;
        }

      break;
    }

  writeReg(REG_PMU_RANGE, range);
}

void BMA250E::setBandwidth(BW_T bw)
{
  writeReg(REG_PMU_BW, bw);
}

void BMA250E::setPowerMode(POWER_MODE_T power)
{
  // mask off reserved bits
  uint8_t reg = readReg(REG_PMU_LPW) & ~_PMU_LPW_RESERVED_MASK;

  reg &= ~(_PMU_LPW_POWER_MODE_MASK << _PMU_LPW_POWER_MODE_SHIFT);
  reg |= (power << _PMU_LPW_POWER_MODE_SHIFT);

  writeReg(REG_PMU_LPW, power);
}

void BMA250E::fifoSetWatermark(int wm)
{
  // do nothing if we don't have a FIFO
  if (!m_fifoAvailable)
    return;

  // mask off illegal values
  uint8_t reg = uint8_t(wm) & _FIFO_CONFIG_0_WATER_MARK_MASK;

  writeReg(REG_FIFO_CONFIG_0, reg);
}

void BMA250E::fifoConfig(FIFO_MODE_T mode, FIFO_DATA_SEL_T axes)
{
  // do nothing if we don't have a FIFO
  if (!m_fifoAvailable)
    return;

  uint8_t reg = ( (mode << _FIFO_CONFIG_1_FIFO_MODE_SHIFT) |
                  (axes << _FIFO_CONFIG_1_FIFO_DATA_SHIFT) );

  writeReg(REG_FIFO_CONFIG_1, reg);
}

void BMA250E::setSelfTest(bool sign, bool amp, SELFTTEST_AXIS_T axis)
{
  uint8_t reg = (axis << _PMU_SELFTTEST_AXIS_SHIFT);

  if (amp)
    reg |= PMU_SELFTTEST_AMP;

  if (sign)
    reg |= PMU_SELFTTEST_SIGN;

  writeReg(REG_PMU_SELFTEST, reg);
}

uint8_t BMA250E::getInterruptEnable0()
{
  return readReg(REG_INT_EN_0) & ~_INT_EN_0_RESERVED_BITS;
}

void BMA250E::setInterruptEnable0(uint8_t bits)
{
  uint8_t reg = bits & ~_INT_EN_0_RESERVED_BITS;

  writeReg(REG_INT_EN_0, reg);
}

uint8_t BMA250E::getInterruptEnable1()
{
  return readReg(REG_INT_EN_1) & ~_INT_EN_1_RESERVED_BITS;
}

void BMA250E::setInterruptEnable1(uint8_t bits)
{
  uint8_t reg = bits & ~_INT_EN_1_RESERVED_BITS;

  writeReg(REG_INT_EN_1, reg);
}

uint8_t BMA250E::getInterruptEnable2()
{
  return readReg(REG_INT_EN_2) & ~_INT_EN_2_RESERVED_BITS;
}

void BMA250E::setInterruptEnable2(uint8_t bits)
{
  uint8_t reg = bits & ~_INT_EN_2_RESERVED_BITS;

  writeReg(REG_INT_EN_2, reg);
}

uint8_t BMA250E::getInterruptMap0()
{
  return readReg(REG_INT_MAP_0);
}

void BMA250E::setInterruptMap0(uint8_t bits)
{
  writeReg(REG_INT_MAP_0, bits);
}

uint8_t BMA250E::getInterruptMap1()
{
  return readReg(REG_INT_MAP_1) & ~_INT_MAP_1_INT1_RESERVED_BITS;
}

void BMA250E::setInterruptMap1(uint8_t bits)
{
  uint8_t reg = bits & ~_INT_MAP_1_INT1_RESERVED_BITS;

  writeReg(REG_INT_MAP_1, reg);
}

uint8_t BMA250E::getInterruptMap2()
{
  return readReg(REG_INT_MAP_2);
}

void BMA250E::setInterruptMap2(uint8_t bits)
{
  writeReg(REG_INT_MAP_2, bits);
}

uint8_t BMA250E::getInterruptSrc()
{
  return readReg(REG_INT_SRC) & ~_INT_SRC_RESERVED_BITS;
}

void BMA250E::setInterruptSrc(uint8_t bits)
{
  uint8_t reg = bits & ~_INT_SRC_RESERVED_BITS;

  writeReg(REG_INT_SRC, reg);
}

uint8_t BMA250E::getInterruptOutputControl()
{
  return readReg(REG_INT_OUT_CTRL) & ~_INT_OUT_CTRL_INT1_RESERVED_BITS;
}

void BMA250E::setInterruptOutputControl(uint8_t bits)
{
  uint8_t reg = bits & ~_INT_OUT_CTRL_INT1_RESERVED_BITS;

  writeReg(REG_INT_OUT_CTRL, reg);
}

void BMA250E::clearInterruptLatches()
{
  uint8_t reg = readReg(REG_INT_RST_LATCH) & ~_INT_RST_LATCH_RESERVED_BITS;

  reg |= INT_RST_LATCH_RESET_INT;

  writeReg(REG_INT_RST_LATCH, reg);
}

BMA250E::RST_LATCH_T BMA250E::getInterruptLatchBehavior()
{
  uint8_t reg = readReg(REG_INT_RST_LATCH) & ~_INT_RST_LATCH_RESERVED_BITS;

  reg &= (_INT_RST_LATCH_MASK << _INT_RST_LATCH_SHIFT);

  return static_cast<RST_LATCH_T>(reg);
}

void BMA250E::setInterruptLatchBehavior(RST_LATCH_T latch)
{
  uint8_t reg = readReg(REG_INT_RST_LATCH) & ~_INT_RST_LATCH_RESERVED_BITS;

  reg &= ~(_INT_RST_LATCH_MASK << _INT_RST_LATCH_SHIFT);
  reg |= (latch << _INT_RST_LATCH_SHIFT);

  writeReg(REG_INT_RST_LATCH, reg);
}

void BMA250E::enableRegisterShadowing(bool shadow)
{
  uint8_t reg = readReg(REG_ACC_HBW) & ~_ACC_HBW_RESERVED_BITS;

  if (shadow)
    reg &= ~ACC_HBW_SHADOW_DIS;
  else
    reg |= ACC_HBW_SHADOW_DIS;

  writeReg(REG_ACC_HBW, reg);
}

void BMA250E::enableOutputFiltering(bool filter)
{
  uint8_t reg = readReg(REG_ACC_HBW) & ~_ACC_HBW_RESERVED_BITS;

  if (filter)
    reg &= ~ACC_HBW_DATA_HIGH_BW;
  else
    reg |= ACC_HBW_DATA_HIGH_BW;

  writeReg(REG_ACC_HBW, reg);
}

uint8_t BMA250E::getInterruptStatus0()
{
  return readReg(REG_INT_STATUS_0);
}

uint8_t BMA250E::getInterruptStatus1()
{
  return readReg(REG_INT_STATUS_1) & ~_INT_STATUS_1_RESERVED_BITS;
}

uint8_t BMA250E::getInterruptStatus2()
{
  return readReg(REG_INT_STATUS_2);
}

uint8_t BMA250E::getInterruptStatus3Bits()
{
  // filter out the orientation bitfield..
  return readReg(REG_INT_STATUS_3) &
    ~(_INT_STATUS_3_ORIENT_MASK << _INT_STATUS_3_ORIENT_SHIFT);
}

BMA250E::ORIENT_T BMA250E::getInterruptStatus3Orientation()
{
  // grab just the orientation bitfield
  uint8_t reg = readReg(REG_INT_STATUS_3) &
    (_INT_STATUS_3_ORIENT_MASK << _INT_STATUS_3_ORIENT_SHIFT);

  reg >>= _INT_STATUS_3_ORIENT_SHIFT;

  return static_cast<ORIENT_T>(reg);
}

void BMA250E::setLowPowerMode2()
{
  uint8_t reg = readReg(REG_PMU_LOW_POWER) & ~_LOW_POWER_RESERVED_BITS;

  // we simply set the low power mode to 2.  Low power mode 1 slows
  // down register write accesses, and we can't handle that.  In the
  // words of the late Admiral Akbar: "We cannot handle firepower of
  // that magnitude!" :(

  reg |= LOW_POWER_LOWPOWER_MODE;

  writeReg(REG_PMU_LOW_POWER, reg);
}

#if defined(SWIGJAVA) || (JAVACALLBACK)
void BMA250E::installISR(INTERRUPT_PINS_T intr, int gpio, mraa::Edge level,
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
void BMA250E::installISR(INTERRUPT_PINS_T intr, int gpio, mraa::Edge level,
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

void BMA250E::uninstallISR(INTERRUPT_PINS_T intr)
{
  if (getPin(intr))
    {
      getPin(intr)->isrExit();
      delete getPin(intr);

      getPin(intr) = 0;
    }
}

mraa::Gpio*& BMA250E::getPin(INTERRUPT_PINS_T intr)
{
  switch(intr)
    {
    case INTERRUPT_INT1:
      return m_gpioIntr1;
      break;

    case INTERRUPT_INT2:
      return m_gpioIntr2;
      break;

    default:
      throw std::out_of_range(string(__FUNCTION__) +
                              ": Invalid interrupt enum passed");
    }
}
