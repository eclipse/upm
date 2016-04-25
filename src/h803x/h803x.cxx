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
#include <assert.h>
#include <errno.h>
#include <iostream>
#include <stdexcept>
#include <string>

#include "h803x.hpp"

using namespace upm;
using namespace std;

// We can't use the modbus float conversion functions since they
// assume the first word is the LSW.  On this device, the first word
// is MSW.  In addition, the data is already IEEE 754 formatted, which
// won't work with just the bit shuffling modbus_get_float*() does.
static float regs2float(uint16_t h, uint16_t l)
{
  // this function will fail horribly if the following isn't true
  assert(sizeof(float) == sizeof(uint32_t));

  // we can't use a cast here, since the data is already IEEE 754
  // formatted, so use a union instead.
  union {
    uint32_t i;
    float f;
  } converter;

  converter.i = ((uint32_t)h << 16) | l;
  return converter.f;
}


H803X::H803X(std::string device, int address, int baud, int bits, char parity,
               int stopBits) :
  m_mbContext(0)
{
  // check some of the parameters
  if (!(bits == 7 || bits == 8))
    {
      throw std::out_of_range(std::string(__FUNCTION__)
                              + ": bits must be 7 or 8");
    }

  if (!(parity == 'N' || parity == 'E' || parity == 'O'))
    {
      throw std::out_of_range(std::string(__FUNCTION__)
                              + ": parity must be 'N', 'O', or 'E'");
    }

  if (!(stopBits == 1 || stopBits == 2))
    {
      throw std::out_of_range(std::string(__FUNCTION__)
                              + ": stopBits must be 1 or 2");
    }

  // now, open/init the device and modbus context

  if (!(m_mbContext = modbus_new_rtu(device.c_str(), baud, parity, bits,
                                     stopBits)))
    {
      throw std::runtime_error(std::string(__FUNCTION__)
                               + ": modbus_new_rtu() failed");
    }

  // set the slave address of the device we want to talk to

  // addresses are only 8bits wide
  address &= 0xff;
  if (modbus_set_slave(m_mbContext, address))
    {
      throw std::runtime_error(std::string(__FUNCTION__)
                               + ": modbus_set_slave() failed");
    }

  // set the serial mode
  modbus_rtu_set_serial_mode(m_mbContext, MODBUS_RTU_RS232);

  // now connect..
  if (modbus_connect(m_mbContext))
    {
      throw std::runtime_error(std::string(__FUNCTION__)
                               + ": modbus_connect() failed");
    }

  // will set m_isH8036 appropriately
  testH8036();

  clearData();

  // turn off debugging
  setDebug(false);
}

H803X::~H803X()
{
  if (m_mbContext)
    {
      modbus_close(m_mbContext);
      modbus_free(m_mbContext);
    }
}

int H803X::readHoldingRegs(HOLDING_REGS_T reg, int len, uint16_t *buf)
{
  int rv;
  int retries = 5;

  // Sometimes it seems the device goes to sleep, and therefore a read
  // will timeout, so we will retry up to 5 times.

  while (retries >= 0)
    {
      if ((rv = modbus_read_registers(m_mbContext, reg, len, buf)) < 0)
        {
          if (errno == ETIMEDOUT)
            {
              // timeout
              retries--;
              sleep(1);
            }
          else if (errno == EMBXILADD)
            {
              // invalid registers will return a EMBXILADD (modbus)
              // error.  We want to detect these as a way to determine
              // whether we are dealing with an H8035 or H8036.
              return -1;
            }
          else
            {
              // anything else is a failure.
              throw std::runtime_error(std::string(__FUNCTION__)
                                       + ": modbus_read_registers() failed: "
                                       + modbus_strerror(errno));
            }
        }
      else
        return rv;              // success
    }

  // if we're here, then all the retries were exhausted
  throw std::runtime_error(std::string(__FUNCTION__)
                           + ": modbus_read_registers() timed out after "
                           + "5 retries");
}

void H803X::writeHoldingReg(HOLDING_REGS_T reg, int value)
{
  if (modbus_write_register(m_mbContext, reg, value) != 1)
    {
      throw std::runtime_error(std::string(__FUNCTION__)
                               + ": modbus_write_register() failed: "
                               + modbus_strerror(errno));
    }
}

void H803X::update()
{
  static const int h8035NumRegs = 4; // 2 regs * 2
  static const int h8036NumRegs = 52; // 26 regs * 2

  int numRegs = (isH8036() ? h8036NumRegs : h8035NumRegs);

  uint16_t buf[numRegs];

  // This should only fail (return -1) if we got isH8036() wrong
  if (readHoldingRegs(HOLDING_CONSUMPTION_KWH, numRegs, buf) < 0)
    {
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": readHoldingRegs() failed: "
                               + modbus_strerror(errno));
    }

  // And so it begins...

  // H8035 / H8036
  m_consumptionkWh              = regs2float(buf[0], buf[1]);
  m_realPowerkW                 = regs2float(buf[2], buf[3]);
  
  // H8036 only
  if (isH8036())
    {
      m_reactivePowerkVAR       = regs2float(buf[4], buf[5]);
      m_apparentPowerkVA        = regs2float(buf[6], buf[7]);
      m_powerFactor             = regs2float(buf[8], buf[9]);
      
      m_voltsLineToLine         = regs2float(buf[10], buf[11]);
      m_voltsLineToNeutral      = regs2float(buf[12], buf[13]);
      
      m_current                 = regs2float(buf[14], buf[15]);
      
      m_realPowerPhaseAkW       = regs2float(buf[16], buf[17]);
      m_realPowerPhaseBkW       = regs2float(buf[18], buf[19]);
      m_realPowerPhaseCkW       = regs2float(buf[20], buf[21]);
      
      m_powerFactorPhaseA       = regs2float(buf[22], buf[23]);
      m_powerFactorPhaseB       = regs2float(buf[24], buf[25]);
      m_powerFactorPhaseC       = regs2float(buf[26], buf[27]);
      
      m_voltsPhaseAB            = regs2float(buf[28], buf[29]);
      m_voltsPhaseBC            = regs2float(buf[30], buf[31]);
      m_voltsPhaseAC            = regs2float(buf[32], buf[33]);
      m_voltsPhaseAN            = regs2float(buf[34], buf[35]);
      m_voltsPhaseBN            = regs2float(buf[36], buf[37]);
      m_voltsPhaseCN            = regs2float(buf[38], buf[39]);
      
      m_currentPhaseA           = regs2float(buf[40], buf[41]);
      m_currentPhaseB           = regs2float(buf[42], buf[43]);
      m_currentPhaseC           = regs2float(buf[44], buf[45]);
      
      m_avgRealPowerkW          = regs2float(buf[46], buf[47]);
      m_minRealPowerkW          = regs2float(buf[48], buf[49]);
      m_maxRealPowerkW          = regs2float(buf[50], buf[51]);
    }
}

string H803X::getSlaveID()
{
  uint8_t id[MODBUS_MAX_PDU_LENGTH];
  int rv;

  if ((rv = modbus_report_slave_id(m_mbContext, MODBUS_MAX_PDU_LENGTH, id)) < 0)
    {
      throw std::runtime_error(std::string(__FUNCTION__)
                               + ": modbus_report_slave_id() failed: "
                               + modbus_strerror(errno));

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

void H803X::setSlaveAddress(int addr)
{
  // addresses are only 8bits wide
  addr &= 0xff;

  if (modbus_set_slave(m_mbContext, addr))
    {
      throw std::runtime_error(std::string(__FUNCTION__)
                               + ": modbus_set_slave() failed: "
                               + modbus_strerror(errno));
    }

  // retest H8036
  testH8036();

  // clear out any previously stored data
  clearData();
}

void H803X::setDebug(bool enable)
{
  m_debugging = enable;

  if (enable)
    modbus_set_debug(m_mbContext, 1);
  else
    modbus_set_debug(m_mbContext, 0);
}

void H803X::clearData()
{
    // H8035
    m_consumptionkWh = 0.0;
    m_realPowerkW = 0.0;

    // H8036
    m_reactivePowerkVAR = 0.0;
    m_apparentPowerkVA = 0.0;
    m_powerFactor = 0.0;
    m_voltsLineToLine = 0.0;
    m_voltsLineToNeutral = 0.0;
    m_current = 0.0;
    m_realPowerPhaseAkW = 0.0;
    m_realPowerPhaseBkW = 0.0;
    m_realPowerPhaseCkW = 0.0;
    m_powerFactorPhaseA = 0.0;
    m_powerFactorPhaseB = 0.0;
    m_powerFactorPhaseC = 0.0;
    m_voltsPhaseAB = 0.0;
    m_voltsPhaseBC = 0.0;
    m_voltsPhaseAC = 0.0;
    m_voltsPhaseAN = 0.0;
    m_voltsPhaseBN = 0.0;
    m_voltsPhaseCN = 0.0;
    m_currentPhaseA = 0.0;
    m_currentPhaseB = 0.0;
    m_currentPhaseC = 0.0;
    m_avgRealPowerkW = 0.0;
    m_minRealPowerkW = 0.0;
    m_maxRealPowerkW = 0.0;
}

void H803X::testH8036()
{
  // here we test a register read to see if we are on an H8036 device,
  // which can provide much more information.

  uint16_t regs[2];

  // here, we'll read 2 registers that only exist on the H8036.  Any
  // failure other than a illegal data access will generate an
  // exception.  A valid request will return >0, and an illegal
  // register read will return -1.
  if (readHoldingRegs(HOLDING_REACTIVE_POWER_KVAR, 2, regs) == -1)
    m_isH8036 = false;
  else
    m_isH8036 = true;
}

void H803X::presetConsumption(float value, MULTIPLIERS_T multiplier)
{
  uint32_t i = uint32_t(value * float(multiplier));

  uint16_t h = uint16_t(i >> 16);
  uint16_t l = uint16_t(i & 0xffff);

  // always write the LSW first
  writeHoldingReg(HOLDING_CONSUMPTION_KWH_INT_L, l);
  writeHoldingReg(HOLDING_CONSUMPTION_KWH_INT_H, h);
}
