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
  return (readWord(ADS1015_REG_CONVERSION) & 0x0fff);
}

float ADS1015::valueToVolts(uint16_t val)
{
  return (val * m_vref / ADS1015_RESOLUTION);
}
