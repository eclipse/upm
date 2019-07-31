/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <string>
#include <stdexcept>

#include "upm_string_parser.hpp"
#include "adc121c021.hpp"

using namespace upm;
using namespace std;


ADC121C021::ADC121C021(int bus, uint8_t address, float vref)
{
  // setup our i2c link
  m_i2c = mraa_i2c_init(bus);
  if (!m_i2c)
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

ADC121C021::ADC121C021(std::string initStr) : mraaIo(initStr)
{
  mraa_io_descriptor* descs = mraaIo.getMraaDescriptors();

  std::vector<std::string> upmTokens;

  if (!mraaIo.getLeftoverStr().empty()) {
      upmTokens = UpmStringParser::parse(mraaIo.getLeftoverStr());
  }

  if(!descs->i2cs)
  {
    throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_i2c_init() failed");
  }
  else
  {
    if( !(m_i2c = descs->i2cs[0]) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_i2c_init() failed");

    }
  }

  std::string::size_type sz;

  for (std::string tok : upmTokens) {
    if(tok.substr(0, 5) == "vref:") {
        m_vref = std::stof(tok.substr(5));
    }
    if(tok.substr(0, 10) == "writeByte:") {
      uint8_t reg = std::stoul(tok.substr(10), &sz, 0);
      tok = tok.substr(10);
      uint8_t byte = std::stoul(tok.substr(sz+1), nullptr, 0);
      writeByte(reg, byte);
    }
    if(tok.substr(0,10) == "writeWord:") {
      uint8_t reg = std::stoul(tok.substr(10), &sz, 0);
      tok = tok.substr(10);
      uint16_t word = std::stoul(tok.substr(sz+1), nullptr, 0);
      writeWord(reg, word);
    }
    if(tok.substr(0,16) == "enableAlertFlag:") {
      bool enable = std::stoi(tok.substr(16), nullptr, 0);
      enableAlertFlag(enable);
    }
    if(tok.substr(0,15) == "enableAlertPin:") {
      bool enable = std::stoi(tok.substr(15), nullptr, 0);
      enableAlertPin(enable);
    }
    if(tok.substr(0,16) == "enableAlertHold:") {
      bool enable = std::stoi(tok.substr(16), nullptr, 0);
      enableAlertHold(enable);
    }
    if(tok.substr(0,27) == "enableAlertPinPolarityHigh:") {
      bool enable = std::stoi(tok.substr(27), nullptr, 0);
      enableAlertPinPolarityHigh(enable);
    }
    if(tok.substr(0,17) == "setAlertLowLimit:") {
      uint16_t limit = std::stoul(tok.substr(17), nullptr, 0);
      setAlertLowLimit(limit);
    }
    if(tok.substr(0,18) == "setAlertHighLimit:") {
      uint16_t limit = std::stoul(tok.substr(18), nullptr, 0);
      setAlertHighLimit(limit);
    }
    if(tok.substr(0,14) == "setHysteresis:") {
      uint16_t limit = std::stoul(tok.substr(14), nullptr, 0);
      setHysteresis(limit);
    }
  }
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
  int val = mraa_i2c_read_byte_data(m_i2c, reg);
  if (val != -1) {
    return (uint8_t) val;
  }
  return 0;
}

uint16_t ADC121C021::readWord(uint8_t reg)
{
  int x = mraa_i2c_read_word_data(m_i2c, reg);
  if (x == -1) {
    return 0;
  }
  uint16_t val = (uint16_t) x;
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
