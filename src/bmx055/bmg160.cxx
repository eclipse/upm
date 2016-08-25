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

#include "bmg160.hpp"

using namespace upm;
using namespace std;

#define BMG160_DEFAULT_CHIPID 0x0f

// conversion from celsius to fahrenheit

static float c2f(float c)
{
  return (c * (9.0 / 5.0) + 32.0);
}

BMG160::BMG160(int bus, int addr, int cs) :
  m_i2c(0), m_spi(0), m_gpioIntr1(0), m_gpioIntr2(0), m_gpioCS(0)
{
  m_gyrX = 0;
  m_gyrY = 0;
  m_gyrZ = 0;
  m_gyrScale = 0;
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
  if (chipID != BMG160_DEFAULT_CHIPID)
    {
      throw std::runtime_error(string(__FUNCTION__)
                               + ": invalid chip ID.  Expected "
                               + std::to_string(int(BMG160_DEFAULT_CHIPID))
                               + ", got "
                               + std::to_string(int(chipID)));
    }

  // call init with default options
  init();
}

BMG160::~BMG160()
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

void BMG160::init(POWER_MODE_T pwr, RANGE_T range, BW_T bw)
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

  // use the FIFO by default
  fifoConfig(FIFO_MODE_BYPASS, FIFO_DATA_SEL_XYZ);
  enableFIFO(true);

  // settle
  usleep(50000);
}

void BMG160::update()
{
  int bufLen = 7; // max, non-FIFO
  uint8_t startReg = REG_RATE_X_LSB;

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

  int16_t val;

  // x
  val = int16_t(buf[1] << 8 | buf[0]);
  m_gyrX = float(val);

  // y
  val = int16_t(buf[3] << 8 | buf[2]);
  m_gyrY = float(val);

  // z
  val = int16_t(buf[5] << 8 | buf[4]);
  m_gyrZ = float(val);

  // get the temperature...

  uint8_t temp = 0;
  if (m_useFIFO)
    {
      // we have to read temperature separately...
      temp = readReg(REG_TEMP);
    }
  else
    {
      // we already got it
      temp = buf[6];
    }

  // .5K/LSB, 23C center point
  m_temperature = (float(temp) / 2.0) + 23.0;
}

void BMG160::enableFIFO(bool useFIFO)
{
  m_useFIFO = useFIFO;
}

uint8_t BMG160::readReg(uint8_t reg)
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

int BMG160::readRegs(uint8_t reg, uint8_t *buffer, int len)
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

void BMG160::writeReg(uint8_t reg, uint8_t val)
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

void BMG160::csOn()
{
  if (m_gpioCS)
    m_gpioCS->write(0);
}

void BMG160::csOff()
{
  if (m_gpioCS)
    m_gpioCS->write(1);
}

uint8_t BMG160::getChipID()
{
  return readReg(REG_CHIP_ID);
}

void BMG160::getGyroscope(float *x, float *y, float *z)
{
  if (x)
    *x = (m_gyrX * m_gyrScale) / 1000.0;

  if (y)
    *y = (m_gyrY * m_gyrScale) / 1000.0;

  if (z)
    *z = (m_gyrZ * m_gyrScale) / 1000.0;
}

float *BMG160::getGyroscope()
{
  static float v[3];

  getGyroscope(&v[0], &v[1], &v[2]);
  return v;
}

float BMG160::getTemperature(bool fahrenheit)
{
  if (fahrenheit)
    return c2f(m_temperature);
  else
    return m_temperature;
}

void BMG160::reset()
{
  writeReg(REG_SOFTRESET, BMG160_RESET_BYTE);
  sleep(1);
}

void BMG160::setRange(RANGE_T range)
{
  switch(range)
    {
    case RANGE_125:
      m_gyrScale = 3.8; // milli-degrees
      break;

    case RANGE_250:
      m_gyrScale = 7.6;
      break;

    case RANGE_500:
      m_gyrScale = 15.3;
      break;

    case RANGE_1000:
      m_gyrScale = 30.5;
      break;

    case RANGE_2000:
      m_gyrScale = 61.0;
      break;
    }

  // we also have to write a fixed '0x10' to the high-order bits for
  // some reason (according to datasheet)
  uint8_t reg = range | (_GYR_RANGE_FIXED_VALUE << _GYR_RANGE_FIXED_SHIFT);
  writeReg(REG_GYR_RANGE, reg);
}

void BMG160::setBandwidth(BW_T bw)
{
  writeReg(REG_GYR_BW, bw);
}

void BMG160::setPowerMode(POWER_MODE_T power)
{
  // mask off reserved bits
  uint8_t reg = readReg(REG_LPM1) & ~_LPM1_RESERVED_MASK;

  reg &= ~(_LPM1_POWER_MODE_MASK << _LPM1_POWER_MODE_SHIFT);
  reg |= (power << _LPM1_POWER_MODE_SHIFT);

  writeReg(REG_LPM1, power);
}

void BMG160::fifoSetWatermark(int wm)
{
  // mask off illegal values
  uint8_t reg = uint8_t(wm) & _FIFO_CONFIG_0_WATER_MARK_MASK;

  writeReg(REG_FIFO_CONFIG_0, reg);
}

void BMG160::fifoConfig(FIFO_MODE_T mode, FIFO_DATA_SEL_T axes)
{
  uint8_t reg = ( (mode << _FIFO_CONFIG_1_FIFO_MODE_SHIFT) |
                  (axes << _FIFO_CONFIG_1_FIFO_DATA_SHIFT) );

  writeReg(REG_FIFO_CONFIG_1, reg);
}

uint8_t BMG160::getInterruptEnable0()
{
  return readReg(REG_INT_EN_0) & ~_INT_EN_0_RESERVED_BITS;
}

void BMG160::setInterruptEnable0(uint8_t bits)
{
  uint8_t reg = bits & ~_INT_EN_0_RESERVED_BITS;

  writeReg(REG_INT_EN_0, reg);
}

uint8_t BMG160::getInterruptMap0()
{
  return readReg(REG_INT_MAP_0) & ~_INT_MAP_0_RESERVED_BITS;
}

void BMG160::setInterruptMap0(uint8_t bits)
{
  uint8_t reg = bits & ~_INT_MAP_0_RESERVED_BITS;

  writeReg(REG_INT_MAP_0, reg);
}

uint8_t BMG160::getInterruptMap1()
{
  return readReg(REG_INT_MAP_1);
}

void BMG160::setInterruptMap1(uint8_t bits)
{
  writeReg(REG_INT_MAP_1, bits);
}

// REG_INT_EN1, for some strange reason
uint8_t BMG160::getInterruptSrc()
{
  return readReg(REG_INT_EN_1) & ~_INT_EN_1_INT1_RESERVED_BITS;
}

void BMG160::setInterruptSrc(uint8_t bits)
{
  uint8_t reg = bits & ~_INT_EN_1_INT1_RESERVED_BITS;

  writeReg(REG_INT_EN_1, reg);
}

uint8_t BMG160::getInterruptOutputControl()
{
  return readReg(REG_INT_EN_1) & ~_INT_EN_1_INT1_RESERVED_BITS;
}

void BMG160::setInterruptOutputControl(uint8_t bits)
{
  uint8_t reg = bits & ~_INT_EN_1_INT1_RESERVED_BITS;

  writeReg(REG_INT_EN_1, reg);
}

void BMG160::clearInterruptLatches()
{
  uint8_t reg = readReg(REG_INT_RST_LATCH) & ~_INT_RST_LATCH_RESERVED_BITS;

  reg |= INT_RST_LATCH_RESET_INT;

  writeReg(REG_INT_RST_LATCH, reg);
}

BMG160::RST_LATCH_T BMG160::getInterruptLatchBehavior()
{
  uint8_t reg = readReg(REG_INT_RST_LATCH) & ~_INT_RST_LATCH_RESERVED_BITS;

  reg &= (_INT_RST_LATCH_MASK << _INT_RST_LATCH_SHIFT);

  return static_cast<RST_LATCH_T>(reg);
}

void BMG160::setInterruptLatchBehavior(RST_LATCH_T latch)
{
  uint8_t reg = readReg(REG_INT_RST_LATCH) & ~_INT_RST_LATCH_RESERVED_BITS;

  reg &= ~(_INT_RST_LATCH_MASK << _INT_RST_LATCH_SHIFT);
  reg |= (latch << _INT_RST_LATCH_SHIFT);

  writeReg(REG_INT_RST_LATCH, reg);
}

void BMG160::enableRegisterShadowing(bool shadow)
{
  uint8_t reg = readReg(REG_RATE_HBW) & ~_RATE_HBW_RESERVED_BITS;

  if (shadow)
    reg &= ~RATE_HBW_SHADOW_DIS;
  else
    reg |= RATE_HBW_SHADOW_DIS;

  writeReg(REG_RATE_HBW, reg);
}

void BMG160::enableOutputFiltering(bool filter)
{
  uint8_t reg = readReg(REG_RATE_HBW) & ~_RATE_HBW_RESERVED_BITS;

  if (filter)
    reg &= ~RATE_HBW_DATA_HIGH_BW;
  else
    reg |= RATE_HBW_DATA_HIGH_BW;

  writeReg(REG_RATE_HBW, reg);
}

uint8_t BMG160::getInterruptStatus0()
{
  return readReg(REG_INT_STATUS_0) & ~_INT_STATUS_0_RESERVED_BITS;
}

uint8_t BMG160::getInterruptStatus1()
{
  return readReg(REG_INT_STATUS_1) & ~_INT_STATUS_1_RESERVED_BITS;
}

uint8_t BMG160::getInterruptStatus2()
{
  return readReg(REG_INT_STATUS_2) & ~_INT_STATUS_2_RESERVED_BITS;
}

uint8_t BMG160::getInterruptStatus3()
{
  return readReg(REG_INT_STATUS_3) & ~_INT_STATUS_3_RESERVED_BITS;
}

#if defined(SWIGJAVA) || (JAVACALLBACK)
void BMG160::installISR(INTERRUPT_PINS_T intr, int gpio, mraa::Edge level,
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
void BMG160::installISR(INTERRUPT_PINS_T intr, int gpio, mraa::Edge level,
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

void BMG160::uninstallISR(INTERRUPT_PINS_T intr)
{
  if (getPin(intr))
    {
      getPin(intr)->isrExit();
      delete getPin(intr);

      getPin(intr) = 0;
    }
}

mraa::Gpio*& BMG160::getPin(INTERRUPT_PINS_T intr)
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
