/*
 * Author: Jon Trulson <jtrulson@ics.com>
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
#include <stdexcept>

#include "adc121c021.h"

using namespace upm;
using namespace std;


ADC121C021::ADC121C021(int bus, uint8_t address, float vref)
{
  // setup our i2c link
  m_i2c = mraa_i2c_init(bus);
  if ( !(m_i2c = mraa_i2c_init(bus)) ) 
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_i2c_init() failed");
      return;
    }

  m_addr = address;

  mraa_result_t ret = mraa_i2c_address(m_i2c, m_addr);

  if (ret != MRAA_SUCCESS)
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_i2c_address() failed");
      return;
    }

  m_vref = vref;
}

ADC121C021::~ADC121C021()
{
  mraa_i2c_stop(m_i2c);
}

mraa_result_t ADC121C021::writeByte(uint8_t reg, uint8_t byte)
{
  return mraa_i2c_write_byte_data(m_i2c, byte, reg);
}

mraa_result_t ADC121C021::writeWord(uint8_t reg, uint16_t word)
{
  // We need to swap the bytes
  uint8_t b1 = (word & 0xff00) >> 8;
  word <<= 8;
  word |= b1;

  return mraa_i2c_write_word_data(m_i2c, word, reg);
}

uint8_t ADC121C021::readByte(uint8_t reg)
{
  return mraa_i2c_read_byte_data(m_i2c, reg);
}

uint16_t ADC121C021::readWord(uint8_t reg)
{
  uint16_t val = mraa_i2c_read_word_data(m_i2c, reg);
  uint8_t b1;

  // The value returned is in the wrong byte order, so we need to swap them
  b1 = (val & 0xff00) >> 8;
  val <<= 8;
  val |= b1;

  return val;
}

uint16_t ADC121C021::value()
{
  // mask off alert flag and reserved bits
  return (readWord(ADC121C021_REG_RESULT) & 0x0fff);
}

float ADC121C021::valueToVolts(uint16_t val)
{
  // The arduino example multiplies this by 2, which seems wrong.  If
  // the reference voltage is 3.0, then you should never get a voltage
  // value higher than that.
  //
  // val * m_vref * 2.0 / ADC121C021_RESOLUTION
  return (val * m_vref / ADC121C021_RESOLUTION);
}

bool ADC121C021::getAlertStatus()
{
  // high order bit is the alert flag, mask off the rest
  bool rv = (readWord(ADC121C021_REG_RESULT) & 0x8000) ? true : false;

  if (rv)
    {
      // read the alert low and high values and set the appropriate
      // member variables
      uint8_t astatus = readByte(ADC121C021_REG_ALERT_STATUS);
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

void ADC121C021::clearAlertStatus()
{
  // zero out both the low and high alert flags
  writeByte(ADC121C021_REG_ALERT_STATUS, 0x03);

  m_alertHigh = false;
  m_alertLow = false;
}

void ADC121C021::enableAlertFlag(bool enable)
{
  // read the current config register
  uint8_t val = readByte(ADC121C021_REG_CONFIG);

  if (enable)
    val |= 0x08;
  else
    val &= ~0x08;

  // write the register back
  writeByte(ADC121C021_REG_CONFIG, val);
}

void ADC121C021::enableAlertPin(bool enable)
{
  // read the current config register
  uint8_t val = readByte(ADC121C021_REG_CONFIG);

  if (enable)
    val |= 0x04;
  else
    val &= ~0x04;

  // write the register back
  writeByte(ADC121C021_REG_CONFIG, val);
}

void ADC121C021::enableAlertHold(bool enable)
{
  // read the current config register
  uint8_t val = readByte(ADC121C021_REG_CONFIG);

  if (enable)
    val |= 0x10;
  else
    val &= ~0x10;

  // write the register back
  writeByte(ADC121C021_REG_CONFIG, val);
}

void ADC121C021::enableAlertPinPolarityHigh(bool enable)
{
  // read the current config register
  uint8_t val = readByte(ADC121C021_REG_CONFIG);

  if (enable)
    val |= 0x01;
  else
    val &= ~0x01;

  // write the register back
  writeByte(ADC121C021_REG_CONFIG, val);
}

void ADC121C021::setAutomaticConversion(ADC121C021_CYCLE_TIME_T cycleTime)
{
  // first we 

  // read the current config register, masking off the cycle time bits
  uint8_t val = readByte(ADC121C021_REG_CONFIG) & 0x1f;

  val |= ((uint8_t)cycleTime << 5);

  // write the register back
  writeByte(ADC121C021_REG_CONFIG, val);
}

mraa_result_t ADC121C021::setAlertLowLimit(uint16_t limit)
{
  // mask off the invalid bits in case they were set
  limit &= 0x0fff;

  // write it
  return writeWord(ADC121C021_REG_ALERT_LIM_UNDER, limit);
}

mraa_result_t ADC121C021::setAlertHighLimit(uint16_t limit)
{
  // mask off the invalid bits in case they were set
  limit &= 0x0fff;

  // write it
  return writeWord(ADC121C021_REG_ALERT_LIM_OVER, limit);
}

mraa_result_t ADC121C021::setHysteresis(uint16_t limit)
{
  // mask off the invalid bits in case they were set
  limit &= 0x0fff;

  // write it
  return writeWord(ADC121C021_REG_ALERT_HYS, limit);
}

uint16_t ADC121C021::getHighestConversion()
{
  return readWord(ADC121C021_REG_HIGHEST_CONV);
}

uint16_t ADC121C021::getLowestConversion()
{
  return readWord(ADC121C021_REG_LOWEST_CONV);
}

mraa_result_t ADC121C021::clearHighestConversion()
{
  return writeWord(ADC121C021_REG_HIGHEST_CONV, 0x0000);
}

mraa_result_t ADC121C021::clearLowestConversion()
{
  return writeWord(ADC121C021_REG_LOWEST_CONV, 0x0fff);
}
