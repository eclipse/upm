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
#include <sstream>
#include <pthread.h>

#include "t3311.hpp"

using namespace upm;
using namespace std;

// conversion from fahrenheit to celsius and back

static float f2c(float f)
{
  return ((f - 32.0) / (9.0 / 5.0));
}

static float c2f(float c)
{
  return (c * (9.0 / 5.0) + 32.0);
}

// conversion from 1-byte BCD to decimal
static uint8_t bcd2dec(uint8_t bcd)
{
  return (bcd / 16 * 10) + (bcd % 16);
}

T3311::T3311(std::string device, int address, int baud, int bits, char parity,
             int stopBits) :
  m_mbContext(0)
{
  // check some of the parameters
  if (!(bits == 7 || bits == 8))
    {
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": bits must be 7 or 8");
    }

  if (!(parity == 'N' || parity == 'E' || parity == 'O'))
    {
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": parity must be 'N', 'O', or 'E'");
    }

  if (!(stopBits == 1 || stopBits == 2))
    {
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": stopBits must be 1 or 2");
    }

  m_temperature = 0.0;
  m_humidity = 0.0;
  m_computedValue = 0.0;
  m_dewPointTemperature = 0.0;
  m_absoluteHumidity = 0.0;
  m_specificHumidity = 0.0;
  m_mixingRatio = 0.0;
  m_specificEnthalpy = 0.0;

  // addresses are only 8bits wide
  address &= 0xff;

  // now, open/init the device and modbus context

  if (!(m_mbContext = modbus_new_rtu(device.c_str(), baud, parity, bits,
                                     stopBits)))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": modbus_new_rtu() failed");
    }

  // set the slave address of the device we want to talk to
  if (modbus_set_slave(m_mbContext, address))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": modbus_set_slave() failed");
    }

  // set the serial mode
  modbus_rtu_set_serial_mode(m_mbContext, MODBUS_RTU_RS232);

  // now connect..
  if (modbus_connect(m_mbContext))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": modbus_connect() failed");
    }

  // This is a bit of a hack.  The device uses bus power, which isn't
  // provided unless the device has been opened and accessed.  As a
  // result, register reads will usually fail the first time the
  // device is accessed after a power cycle.  Here, we read the
  // temperature value (which will most likely fail), then sleep,
  // allowing the sensor to "boot".  The datasheet says it takes at
  // about 2 seconds to boot, we will wait for 5.
  uint16_t tmp;
  modbus_read_input_registers(m_mbContext, REG_TEMPERATURE, 1, &tmp);

  // sleep for 5 seconds to give time for device to powerup and boot
  sleep(5);

  // turn off debugging
  setDebug(false);

  // now read the UNIT_SETTING reg to see what units we are getting
  // our temperature data in.
  tmp = readInputReg(REG_UNIT_SETTINGS);
  if (tmp & 0x0001)
    m_isCelsius = false;
  else
    m_isCelsius = true;

  // read in the the FW_LO register (BCD encoded) and convert
  tmp = readInputReg(REG_FW_LO);

  // HI byte (major)
  m_fwRevHi = (tmp >> 8) & 0xff;
  m_fwRevHi = bcd2dec(m_fwRevHi);

  // LO byte (minor)
  m_fwRevLo = (tmp & 0xff);
  m_fwRevLo = bcd2dec(m_fwRevLo);

  if (m_fwRevHi >= 2 && m_fwRevLo >= 44)
    m_isExtendedDataAvailable = true;
  else
    m_isExtendedDataAvailable = false;

  // now get the serial number (BCD encoded 4-byte value, which we
  // will pack into a string)
  stringstream preformat;
  uint8_t b;
  // LO (but really HI)
  tmp = readInputReg(REG_SERIAL_LO);
  b = bcd2dec((tmp & 0xff00) >> 8);
  preformat << int(b);

  b = bcd2dec(tmp & 0x00ff);
  preformat << int(b);

  // HI (but really LO)
  tmp = readInputReg(REG_SERIAL_HI);

  b = bcd2dec((tmp & 0xff00) >> 8);
  preformat << int(b);
  b = bcd2dec(tmp & 0x00ff);
  preformat << int(b);

  m_serialNumber = preformat.str();
}

T3311::~T3311()
{
  if (m_mbContext)
    {
      modbus_close(m_mbContext);
      modbus_free(m_mbContext);
    }
}

uint16_t T3311::readInputReg(int reg)
{
  uint16_t val;

  if (modbus_read_input_registers(m_mbContext, reg, 1, &val) <= 0)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": modbus_read_input_registers() failed");
    }

  return val;
}

int T3311::readInputRegs(int reg, int len, uint16_t *buf)
{
  int rv;

  if ((rv = modbus_read_input_registers(m_mbContext, reg, len, buf)) < 0)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": modbus_read_input_registers() failed");
    }

  return rv;
}

void T3311::update()
{
  static const int dataLen = 9;
  uint16_t data[dataLen];

  // we read the 9 registers starting at the temperature
  if (readInputRegs(REG_TEMPERATURE, dataLen, data) != dataLen)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": read less than the expected 9 registers");
    }

  // temperature first, we always store as C
  float tmpF = float((int16_t)data[0]) / 10.0;
  if (m_isCelsius)
    m_temperature = tmpF;
  else
    m_temperature = f2c(tmpF);

  m_humidity = float((int16_t)data[1]) / 10.0;
  m_computedValue = float((int16_t)data[2]) / 10.0;

  // skip data[3], (pressure) as this device does not support it

  // if extended info is supported, grab those too
  if (extendedDataAvailable())
    {
      // we always store temps in C
      tmpF = float((int16_t)data[4]) / 10.0;
      if (m_isCelsius)
        m_dewPointTemperature = tmpF;
      else
        m_dewPointTemperature = f2c(tmpF);

      m_absoluteHumidity = float((int16_t)data[5]) / 10.0;
      m_specificHumidity = float((int16_t)data[6]) / 10.0;
      m_mixingRatio = float((int16_t)data[7]) / 10.0;
      m_specificEnthalpy = float((int16_t)data[8]) / 10.0;
    }
}

float T3311::getTemperature(bool fahrenheit)
{
  if (fahrenheit)
    return c2f(m_temperature);
  else
    return m_temperature;
}

float T3311::getHumidity()
{
  return m_humidity;
}

float T3311::getComputedValue()
{
  return m_computedValue;
}

float T3311::getDewPointTemperature(bool fahrenheit)
{
  if (fahrenheit)
    return c2f(m_dewPointTemperature);
  else
    return m_dewPointTemperature;
}

float T3311::getAbsoluteHumidity()
{
  return m_absoluteHumidity;
}

float T3311::getSpecificHumidity()
{
  return m_specificHumidity;
}

float T3311::getMixingRatio()
{
  return m_mixingRatio;
}

float T3311::getSpecificEnthalpy()
{
  return m_specificEnthalpy;
}

void T3311::setDebug(bool enable)
{
  m_debugging = enable;

  if (enable)
    modbus_set_debug(m_mbContext, 1);
  else
    modbus_set_debug(m_mbContext, 0);
}
