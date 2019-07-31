/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <time.h>
#include <stdexcept>

#include "ds2413.hpp"
#include "upm_string_parser.hpp"


using namespace upm;
using namespace std;


DS2413::DS2413(int uart) :
  m_uart(uart)
{
  m_devicesFound = 0;

  // check basic access to the 1-wire bus (presence detect)
  mraa::Result rv;

  if ((rv = m_uart.reset()) != mraa::SUCCESS)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": reset() failed, no devices on bus?");
      return;
    }
}

DS2413::DS2413(std::string initStr) : m_uart(nullptr), mraaIo(initStr)
{
  mraa_io_descriptor* descs = mraaIo.getMraaDescriptors();
  std::vector<std::string> upmTokens;
  
  if(!mraaIo.getLeftoverStr().empty()) {
    upmTokens = UpmStringParser::parse(mraaIo.getLeftoverStr());
  }
  
  m_uart = descs->uart_ows[0];
  m_devicesFound = 0;

  // check basic access to the 1-wire bus (presence detect)
  mraa::Result rv;

  if ((rv = m_uart.reset()) != mraa::SUCCESS)
  {
    throw std::runtime_error(std::string(__FUNCTION__) +
                              ": reset() failed, no devices on bus?");
    return;
  }
  
  for(std::string tok : upmTokens) {
    if(tok.substr(0, 11) == "writeGpios:") {
      std::string::size_type sz;
      int index = std::stoi(tok.substr(11), &sz);
      tok = tok.substr(11);
      int value = std::stoi(tok.substr(sz+1), nullptr, 0);
      writeGpios(index, value);
    }
  }
}

DS2413::~DS2413()
{
}

void DS2413::init()
{
  // iterate through the bus and build up a list of detected DS2413
  // devices (only)

  // empty the map, in case this method has already been run once
  // before
  m_devicesFound = 0;
  m_deviceMap.clear();

  // start the search from scratch
  string id = m_uart.search(true);

  if (id.empty())
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": no devices detected on bus");
      return;
    }

  while (!id.empty())
    {
      // The first byte (id[0]]) is the device type (family) code.  We
      // are only interested in the family code for this device (0x3a).

      if ((uint8_t)id[0] == DS2413_FAMILY_CODE)
        {
          // we have a winner, add it to our map and continue searching

          m_deviceMap[m_devicesFound] = id;
          m_devicesFound++;
        }

      // continue search
      id = m_uart.search(false);
    }

  if (!m_devicesFound)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": no DS2413 devices found on bus");
      return;
    }
}

int DS2413::readGpios(int index)
{
  if (index < 0 || index >= m_devicesFound)
    {
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": device index out of range");
      return 0;
    }

  m_uart.command(ACCESS_READ, m_deviceMap[index]);

  uint8_t value = m_uart.readByte();

  // Validity is performed by taking the high nibble, inverting it, and
  // copmpating it to the low nibble.  If they are equal, then the
  // data is good.

  if ( (value & 0x0f) != ((~value >> 4) & 0x0f) )
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                              ": returned value failed integrity check");
      return 0;
    }

  m_uart.reset();

  // Only the 4 lsb's are relevant
  return (value & 0x0f);
}

void DS2413::writeGpios(int index, int value)
{
  if (index < 0 || index >= m_devicesFound)
    {
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": device index out of range");
      return;
    }

  //  mask out everything but the first 2 bits
  uint8_t val = (uint8_t)value & 0x03;

  // the value must have the upper 8 bits written set to 1's
  val |= 0xfc;

  m_uart.command(ACCESS_WRITE, m_deviceMap[index]);

  // first we write the new value, then the inverted value
  m_uart.writeByte(val);
  m_uart.writeByte(~val);

  // now we read back a response indicating success or failure
  uint8_t resp = m_uart.readByte();

  if (resp != ACK_SUCCESS)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": failed response validation");
      return;
    }

  m_uart.reset();

  return;

}
