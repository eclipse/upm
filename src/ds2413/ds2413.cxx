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

#include <iostream>
#include <time.h>
#include <stdexcept>

#include "ds2413.hpp"

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
