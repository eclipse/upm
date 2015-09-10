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

#include "pca9685.h"

using namespace upm;
using namespace std;


PCA9685::PCA9685(int bus, uint8_t address, bool raw)
{
  m_addr = address;

  // setup our i2c link
  if ( raw )
    {
      m_i2c = mraa_i2c_init_raw(bus);
    }
  else
    {
      m_i2c = mraa_i2c_init(bus);
    }

  if ( !m_i2c)
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_i2c_init() failed");
      return;
    }
      
  mraa_result_t rv;
  
  if ( (rv = mraa_i2c_address(m_i2c, m_addr)) != MRAA_SUCCESS)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": mraa_i2c_address() failed");
      return;
    }

  // enable auto-increment mode by default
  enableAutoIncrement(true);

  // enable restart by default.
  enableRestart(true);
}

PCA9685::~PCA9685()
{
  setModeSleep(true);
  mraa_i2c_stop(m_i2c);
}

bool PCA9685::writeByte(uint8_t reg, uint8_t byte)
{
  mraa_result_t rv = mraa_i2c_write_byte_data(m_i2c, byte, reg);

  if (rv != MRAA_SUCCESS)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": mraa_i2c_write_byte_data() failed");
      return false;
    }

  return true;
}

bool PCA9685::writeWord(uint8_t reg, uint16_t word)
{
  mraa_result_t rv = mraa_i2c_write_word_data(m_i2c, word, reg);

  if (rv != MRAA_SUCCESS)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": mraa_i2c_write_word_data() failed");
      return false;
    }

  return true;
}

uint8_t PCA9685::readByte(uint8_t reg)
{
  return mraa_i2c_read_byte_data(m_i2c, reg);
}

uint16_t PCA9685::readWord(uint8_t reg)
{
  return mraa_i2c_read_word_data(m_i2c, reg);
}

bool PCA9685::setModeSleep(bool sleep)
{
  uint8_t mode1 = readByte(REG_MODE1);
  uint8_t restartBit = mode1 & MODE1_RESTART;

  if (sleep)
    mode1 |= MODE1_SLEEP;
  else
    mode1 &= ~MODE1_SLEEP;

  // if we are waking up, then preserve but don't write restart bit if set
  if (!sleep && restartBit)
    mode1 &= ~MODE1_RESTART;

  writeByte(REG_MODE1, mode1);

  // Need a delay of 500us after turning sleep mode off for the oscillator 
  // to stabilize
  if (!sleep)
    usleep(500);

  // now check to see if we want to (and can) restart when waking up
  if (restartBit && m_restartEnabled && !sleep)
    {
      mode1 |= restartBit;
      writeByte(REG_MODE1, mode1);
    }

  return true;
}

bool PCA9685::enableAutoIncrement(bool ai)
{
  uint8_t mode1 = readByte(REG_MODE1);

  if (ai)
    mode1 |= MODE1_AI;
  else
    mode1 &= ~MODE1_AI;

  return writeByte(REG_MODE1, mode1);
}

bool PCA9685::ledFullOn(uint8_t led, bool val)
{
  if (led > 15 && (led != PCA9685_ALL_LED))
    {
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": led value must be between 0-15 or " +
                              "PCA9685_ALL_LED (255)");
      return false;
    }

  // figure out the register offset (*_ON_H)
  uint8_t regoff;

  if (led == PCA9685_ALL_LED)
    regoff = REG_ALL_LED_ON_H;
  else
    regoff = REG_LED0_ON_L + (led * 4) + 1;

  uint8_t bits = readByte(regoff);

  if (val)
    bits |= 0x10;
  else
    bits &= ~0x10;

  return writeByte(regoff, bits);
}

bool PCA9685::ledFullOff(uint8_t led, bool val)
{
  if (led > 15 && (led != PCA9685_ALL_LED))
    {
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": led value must be between 0-15 or " +
                              "PCA9685_ALL_LED (255)");
      return false;
    }

  // figure out the register offset (*_OFF_H)
  uint8_t regoff;

  if (led == PCA9685_ALL_LED)
    regoff = REG_ALL_LED_OFF_H;
  else
    regoff = REG_LED0_ON_L + (led * 4) + 3;

  uint8_t bits = readByte(regoff);

  if (val)
    bits |= 0x10;
  else
    bits &= ~0x10;

  return writeByte(regoff, bits);
}

bool PCA9685::ledOnTime(uint8_t led, uint16_t time)
{
  if (led > 15 && (led != PCA9685_ALL_LED))
    {
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": led value must be between 0-15 or " +
                              "PCA9685_ALL_LED (255)");
      return false;
    }

  if (time > 4095)
    {
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": time value must be between 0-4095");
      return false;
    }

  // figure out the register offset (*_ON_L)
  uint8_t regoff;

  if (led == PCA9685_ALL_LED)
    regoff = REG_ALL_LED_ON_L;
  else
    regoff = REG_LED0_ON_L + (led * 4);

  // we need to preserve the full ON bit in *_ON_H
  uint8_t onbit = (readByte(regoff + 1) & 0x10);

  time = (time & 0x0fff) | (onbit << 8);

  return writeWord(regoff, time);
}

bool PCA9685::ledOffTime(uint8_t led, uint16_t time)
{
  if (led > 15 && (led != PCA9685_ALL_LED))
    {
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": led value must be between 0-15 or " +
                              "PCA9685_ALL_LED (255)");
      return false;
    }

  if (time > 4095)
    {
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": time value must be between 0-4095");
      return false;
    }

  // figure out the register offset (*_OFF_L)
  uint8_t regoff;

  if (led == PCA9685_ALL_LED)
    regoff = REG_ALL_LED_OFF_L;
  else
    regoff = REG_LED0_ON_L + (led * 4) + 2;

  // we need to preserve the full OFF bit in *_OFF_H
  uint8_t offbit = (readByte(regoff + 1) & 0x10);

  time = (time & 0x0fff) | (offbit << 8);

  return writeWord(regoff, time);
}

bool PCA9685::setPrescale(uint8_t prescale)
{
  // This will be ignored if the device isn't in SLEEP mode
  return writeByte(REG_PRESCALE, prescale);
}

bool PCA9685::setPrescaleFromHz(float hz, float oscFreq)
{
  float prescale = round( oscFreq / (4096.0 * hz) ) - 1;

  return setPrescale(uint8_t(prescale));
}
