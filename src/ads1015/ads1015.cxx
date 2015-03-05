/*
 * Author: Scott Ware <scott.r.ware@intel.com>
 * Copyright (c) 2014 Intel Corporation.
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

#include "ads1015.h"

using namespace upm;
using namespace std;


ADS1015::ADS1015(int bus, uint8_t address, float vref)
{
  // Setup i2c
  m_i2c = mraa_i2c_init(bus);

  m_addr = address;

  mraa_result_t ret = mraa_i2c_address(m_i2c, m_addr);

  if (ret != MRAA_SUCCESS) 
    cerr << "ADS1015: Could not initialize i2c bus. " << endl;

  m_vref = vref;
}

ADS1015::~ADS1015()
{
  mraa_i2c_stop(m_i2c);
}

mraa_result_t ADS1015::writeByte(uint8_t reg, uint8_t byte)
{
  return mraa_i2c_write_byte_data(m_i2c, byte, reg);
}

mraa_result_t ADS1015::writeWord(uint8_t reg, uint16_t word)
{
  // We need to swap the bytes
  uint8_t b1 = (word & 0xff00) >> 8;
  word <<= 8;
  word |= b1;

  return mraa_i2c_write_word_data(m_i2c, word, reg);
}

uint8_t ADS1015::readByte(uint8_t reg)
{
  return mraa_i2c_read_byte_data(m_i2c, reg);
}

uint16_t ADS1015::readWord(uint8_t reg)
{
  uint16_t val = mraa_i2c_read_word_data(m_i2c, reg);
  uint8_t b1;

  // The value returned is in the wrong byte order, so we need to swap them
  b1 = (val & 0xff00) >> 8;
  val <<= 8;
  val |= b1;

  return val;
}

uint16_t ADS1015::value()
{
  // mask off alert flag and reserved bits
  return (readWord(ADS1015_REG_RESULT) & 0x0fff);
}

float ADS1015::valueToVolts(uint16_t val)
{
  return (val * m_vref / ADS1015_RESOLUTION);
}

bool ADS1015::getAlertStatus()
{
  // high order bit is the alert flag, mask off the rest
  bool rv = (readWord(ADS1015_REG_RESULT) & 0x8000) ? true : false;

  if (rv)
    {
      // read the alert low and high values and set the appropriate
      // member variables
      uint8_t astatus = readByte(ADS1015_REG_ALERT_STATUS);
      if (astatus & 0x01)
        m_alertLow = true;
      else 
        m_alertLow = false;

      if (astatus & 0x02)
        m_alertHigh = true;
      else 
        m_alertHigh = false;
    }

  return rv;
}

void ADS1015::clearAlertStatus()
{
  // zero out both the low and high alert flags
  writeByte(ADS1015_REG_ALERT_STATUS, 0x03);

  m_alertHigh = false;
  m_alertLow = false;
}

void ADS1015::enableAlertFlag(bool enable)
{
  // read the current config register
  uint8_t val = readByte(ADS1015_REG_CONFIG);

  if (enable)
    val |= 0x08;
  else
    val &= ~0x08;

  // write the register back
  writeByte(ADS1015_REG_CONFIG, val);
}

void ADS1015::enableAlertPin(bool enable)
{
  // read the current config register
  uint8_t val = readByte(ADS1015_REG_CONFIG);

  if (enable)
    val |= 0x04;
  else
    val &= ~0x04;

  // write the register back
  writeByte(ADS1015_REG_CONFIG, val);
}

void ADS1015::enableAlertHold(bool enable)
{
  // read the current config register
  uint8_t val = readByte(ADS1015_REG_CONFIG);

  if (enable)
    val |= 0x10;
  else
    val &= ~0x10;

  // write the register back
  writeByte(ADS1015_REG_CONFIG, val);
}

void ADS1015::enableAlertPinPolarityHigh(bool enable)
{
  // read the current config register
  uint8_t val = readByte(ADS1015_REG_CONFIG);

  if (enable)
    val |= 0x01;
  else
    val &= ~0x01;

  // write the register back
  writeByte(ADS1015_REG_CONFIG, val);
}

void ADS1015::setAutomaticConversion(ADS1015_CYCLE_TIME_T cycleTime)
{
  // read the current config register, masking off the cycle time bits
  uint8_t val = readByte(ADS1015_REG_CONFIG) & 0x1f;

  val |= ((uint8_t)cycleTime << 5);

  // write the register back
  writeByte(ADS1015_REG_CONFIG, val);
}

mraa_result_t ADS1015::setAlertLowLimit(uint16_t limit)
{
  // mask off the invalid bits in case they were set
  limit &= 0x0fff;

  // write it
  return writeWord(ADS1015_REG_ALERT_LIM_UNDER, limit);
}

mraa_result_t ADS1015::setAlertHighLimit(uint16_t limit)
{
  // mask off the invalid bits in case they were set
  limit &= 0x0fff;

  // write it
  return writeWord(ADS1015_REG_ALERT_LIM_OVER, limit);
}

mraa_result_t ADS1015::setHysteresis(uint16_t limit)
{
  // mask off the invalid bits in case they were set
  limit &= 0x0fff;

  // write it
  return writeWord(ADS1015_REG_ALERT_HYS, limit);
}

uint16_t ADS1015::getHighestConversion()
{
  return readWord(ADS1015_REG_HIGHEST_CONV);
}

uint16_t ADS1015::getLowestConversion()
{
  return readWord(ADS1015_REG_LOWEST_CONV);
}

mraa_result_t ADS1015::clearHighestConversion()
{
  return writeWord(ADS1015_REG_HIGHEST_CONV, 0x0000);
}

mraa_result_t ADS1015::clearLowestConversion()
{
  return writeWord(ADS1015_REG_LOWEST_CONV, 0x0fff);
}
