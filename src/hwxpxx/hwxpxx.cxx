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

#include "hwxpxx.hpp"

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

HWXPXX::HWXPXX(std::string device, int address, int baud, int bits, char parity,
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
  m_slider = 0;

  // now, open/init the device and modbus context

  if (!(m_mbContext = modbus_new_rtu(device.c_str(), baud, parity, bits,
                                     stopBits)))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": modbus_new_rtu() failed");
    }

  // set the slave address of the device we want to talk to

  // addresses are only 8bits wide
  address &= 0xff;
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

  // read the 2 coils to determine temperature scale and current status
  // of (optional) override switch
  uint8_t coils[2];
  readCoils(COIL_TEMP_SCALE, 2, coils);

  // temp scale
  if (coils[0])
    m_isCelsius = false;
  else
    m_isCelsius = true;

  // current override switch status
  m_override = ((coils[1]) ? true : false);

  // turn off debugging
  setDebug(false);
}

HWXPXX::~HWXPXX()
{
  if (m_mbContext)
    {
      modbus_close(m_mbContext);
      modbus_free(m_mbContext);
    }
}

int HWXPXX::readInputRegs(INPUT_REGS_T reg, int len, uint16_t *buf)
{
  int rv;

  if ((rv = modbus_read_input_registers(m_mbContext, reg, len, buf)) < 0)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": modbus_read_input_registers() failed");
    }

  return rv;
}

uint16_t HWXPXX::readInputReg(INPUT_REGS_T reg)
{
  uint16_t val;

  if (readInputRegs(reg, 1, &val) != 1)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": readInputRegs() returned bad data");
    }

  return val;
}

int HWXPXX::readHoldingRegs(HOLDING_REGS_T reg, int len, uint16_t *buf)
{
  int rv;

  if ((rv = modbus_read_registers(m_mbContext, reg, len, buf)) < 0)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": modbus_read_registers() failed");
    }

  return rv;
}

uint16_t HWXPXX::readHoldingReg(HOLDING_REGS_T reg)
{
  uint16_t val;

  if (readHoldingRegs(reg, 1, &val) != 1)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": readInputRegs() returned bad data");
    }

  return val;
}

void HWXPXX::writeHoldingReg(HOLDING_REGS_T reg, int value)
{
  if (modbus_write_register(m_mbContext, reg, value) != 1)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": modbus_write_register() failed");
    }
}

int HWXPXX::readCoils(COIL_REGS_T reg, int numBits, uint8_t *buf)
{
  int rv;

  if ((rv = modbus_read_bits(m_mbContext, reg, numBits, buf)) < 0)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": modbus_read_bits() failed");
    }

  return rv;
}

bool HWXPXX::readCoil(COIL_REGS_T reg)
{
  uint8_t buf;

  if (readCoils(reg, 1, &buf) != 1)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": readCoils() returned bad data");
    }

  return ((buf) ? true : false);
}

void HWXPXX::writeCoil(COIL_REGS_T reg, bool val)
{
  int value = (val) ? TRUE : FALSE;

  if (modbus_write_bit(m_mbContext, reg, value) != 1)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": modbus_write_bit() failed");
    }
}

void HWXPXX::update()
{
  static const int dataLen = 3;
  uint16_t data[dataLen];

  // we read 3 input registers starting at humidity
  if (readInputRegs(INPUT_HUMIDITY, dataLen, data) != dataLen)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": readInputRegs() failed to read 3 registers");
    }

  // humidity
  m_humidity = float((int16_t)data[0]) / 10.0;

  // temperature, we always store as C
  float tmpF = float((int16_t)data[1]) / 10.0;

  if (m_isCelsius)
    m_temperature = tmpF;
  else
    m_temperature = f2c(tmpF);

  // optional slider level
  m_slider = int(data[2]);

  // optional override switch status
  m_override = readCoil(COIL_OVERRIDE);
}

float HWXPXX::getTemperature(bool fahrenheit)
{
  if (fahrenheit)
    return c2f(m_temperature);
  else
    return m_temperature;
}

float HWXPXX::getHumidity()
{
  return m_humidity;
}

int HWXPXX::getSlider()
{
  return m_slider;
}

bool HWXPXX::getOverrideSwitchStatus()
{
  return m_override;
}

int HWXPXX::getTemperatureOffset()
{
  return int((int16_t)readHoldingReg(HOLDING_TEMP_OFFSET));
}

int HWXPXX::getHumidityOffset()
{
  return int((int16_t)readHoldingReg(HOLDING_HUM_OFFSET));
}

void HWXPXX::setTemperatureOffset(int offset)
{
  if (offset < -50 || offset > 50)
    {
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": offset must be between -50 to 50");
    }

  writeHoldingReg(HOLDING_TEMP_OFFSET, offset);
}

void HWXPXX::setHumidityOffset(int offset)
{
  if (offset < -100 || offset > 100)
    {
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": offset must be between -100 to 100");
    }

  writeHoldingReg(HOLDING_HUM_OFFSET, offset);
}

void HWXPXX::clearOverrideSwitch()
{
  writeCoil(COIL_OVERRIDE, false);
}

void HWXPXX::setTemperatureScale(bool fahrenheit)
{
  writeCoil(COIL_TEMP_SCALE, fahrenheit);

  // now re-read and set m_isCelsius properly
  if (readCoil(COIL_TEMP_SCALE))
    m_isCelsius = false;
  else
    m_isCelsius = true;
}

string HWXPXX::getSlaveID()
{
  uint8_t id[MODBUS_MAX_PDU_LENGTH];
  int rv;

  if ((rv = modbus_report_slave_id(m_mbContext, MODBUS_MAX_PDU_LENGTH, id)) < 0)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": modbus_report_slave_id() failed");
    }

  // the first byte is the number of bytes in the response, the second
  // byte is the active indicator (00 = off, ff = on), and the rest
  // are ascii identification (company, model, and serial number) data.

  if (rv > 2)
    {
      string retID((char *)&id[2], rv - 2);
      return retID;
    }
  else
    return "";
}

void HWXPXX::setSlaveAddress(int addr)
{
  // addresses are only 8bits wide
  addr &= 0xff;

  if (modbus_set_slave(m_mbContext, addr))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": modbus_set_slave() failed");
    }

  // now re-read and set m_isCelsius properly
  if (readCoil(COIL_TEMP_SCALE))
    m_isCelsius = false;
  else
    m_isCelsius = true;
}

void HWXPXX::setDebug(bool enable)
{
  m_debugging = enable;

  if (enable)
    modbus_set_debug(m_mbContext, 1);
  else
    modbus_set_debug(m_mbContext, 0);
}
