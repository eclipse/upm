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
#include <syslog.h>

#include "ds18b20.hpp"

using namespace upm;
using namespace std;

// conversion from celsius to fahrenheit
static float c2f(float c)
{
  return (c * (9.0 / 5.0) + 32.0);
}

DS18B20::DS18B20(int uart) :
  m_uart(uart)
{
  m_devicesFound = 0;

  // check basic access to the 1-wire bus (presence detect)
  mraa::Result rv;

  if ((rv = m_uart.reset()) != mraa::SUCCESS)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": reset() failed, no devices on bus?");
    }
}

DS18B20::~DS18B20()
{
}

void DS18B20::init()
{
  // iterate through the bus and build up a list of detected DS18B20
  // devices (only)

  // empty the map, in case this method has already been run once
  // before
  m_devicesFound = 0;
  m_deviceMap.clear();

  sensor_info_t sinfo;

  // defaults
  sinfo.temperature = 0.0;
  sinfo.resolution = RESOLUTION_12BITS;

  // start the search from scratch
  string id = m_uart.search(true);
  if (id.empty())
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": no devices detected on bus");
    }

  while (!id.empty())
    {
      // The first byte (id[0]]) is the device type (family) code.  We
      // are only interested in the family code for these devices.

      if ((uint8_t)id[0] == DS18B20_FAMILY_CODE)
        {
          // we have a winner, add it to our map and continue searching

          sinfo.id = id;
          m_deviceMap[m_devicesFound] = sinfo;

          m_devicesFound++;
        }

      // continue search
      id = m_uart.search(false);
    }

  if (!m_devicesFound)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": no DS18B20 devices found on bus");
    }

  // iterate through the found devices and query their resolutions
  for (int i=0; i<m_devicesFound; i++)
    {
      // read only the first 5 bytes of the scratchpad
      static const int numScratch = 5;
      uint8_t scratch[numScratch];

      m_uart.command(CMD_READ_SCRATCHPAD, m_deviceMap[i].id);
      for (int j=0; j<numScratch; j++)
        scratch[j] = m_uart.readByte();

      // config byte, shift the resolution to bit 0
      scratch[4] >>= _CFG_RESOLUTION_SHIFT;

      switch (scratch[4] & _CFG_RESOLUTION_MASK)
        {
        case 0: m_deviceMap[i].resolution = RESOLUTION_9BITS; break;
        case 1: m_deviceMap[i].resolution = RESOLUTION_10BITS; break;
        case 2: m_deviceMap[i].resolution = RESOLUTION_11BITS; break;
        case 3: m_deviceMap[i].resolution = RESOLUTION_12BITS; break;
        }

      // reset the bus
      m_uart.reset();
    }
}

void DS18B20::update(int index)
{
  if (index >= m_devicesFound)
    {
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": device index out of range");
    }

  // should we update all of them?
  bool doAll = (index < 0) ? true : false;

  if (doAll)
    {
      // if we want to update all of them, we will first send the
      // convert command to all of them, then wait.  This will be
      // faster, timey-wimey wise, then converting, sleeping, and
      // reading each individual sensor.

      for (int i=0; i<m_devicesFound; i++)
        m_uart.command(CMD_CONVERT, m_deviceMap[i].id);
    }
  else
    m_uart.command(CMD_CONVERT, m_deviceMap[index].id);

  // wait for conversion(s) to finish
  usleep(750000); // 750ms max

  if (doAll)
    {
      for (int i=0; i<m_devicesFound; i++)
        m_deviceMap[i].temperature = readSingleTemp(i);
    }
  else
    m_deviceMap[index].temperature = readSingleTemp(index);
}

// utility function to read temp data from a single sensor
float DS18B20::readSingleTemp(int index)
{
  if (index < 0 || index >= m_devicesFound)
    {
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": device index out of range");
    }

  static const int numScratch = 9;
  uint8_t scratch[numScratch];

  // read the 9-byte scratchpad
  m_uart.command(CMD_READ_SCRATCHPAD, m_deviceMap[index].id);
  for (int i=0; i<numScratch; i++)
    scratch[i] = m_uart.readByte();

  // validate cksum -- if we get an error, we will warn and simply
  // return the current (previously read) temperature
  uint8_t crc = m_uart.crc8(scratch, 8);

  if (crc != scratch[8])
    {
      cerr << __FUNCTION__ << ": crc check failed for device "
           << index << ", returning previously measured temperature" << endl;
      return m_deviceMap[index].temperature;
    }

  // check the sign bit(s)
  bool negative = (scratch[1] & 0x80) ? true : false;

  // shift everything into position
  int16_t temp = (scratch[1] << 8) | scratch[0];

  // grab the fractional
  uint8_t frac = temp & 0x0f;

  // depending on the resolution, some frac bits should be ignored, so
  // we mask them off.  For 12bits, all bits are valid so we leve them
  // alone.

  switch (m_deviceMap[index].resolution)
    {
    case RESOLUTION_9BITS: frac &= 0x08; break;
    case RESOLUTION_10BITS: frac &= 0x0c; break;
    case RESOLUTION_11BITS: frac &= 0x0e; break;
    default:
        syslog(LOG_WARNING, "%s: switch case not defined",
            std::string(__FUNCTION__).c_str());
    }

  // remove the fractional with extreme prejudice
  temp >>= 4;

  // compensate for sign
  if (negative)
    temp -= 65536; // 2^^16

  // convert
  return ( float(temp) + (float(frac) * 0.0625) );
}

float DS18B20::getTemperature(int index, bool fahrenheit)
{
  if (index < 0 || index >= m_devicesFound)
    {
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": device index out of range");
    }

  if (fahrenheit)
    return c2f(m_deviceMap[index].temperature);
  else
    return m_deviceMap[index].temperature;
}

void DS18B20::setResolution(int index, RESOLUTIONS_T res)
{
  if (index < 0 || index >= m_devicesFound)
    {
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": device index out of range");
    }

  static const int numScratch = 9;
  uint8_t scratch[numScratch];

  // read the 9-byte scratchpad
  m_uart.command(CMD_READ_SCRATCHPAD, m_deviceMap[index].id);
  for (int i=0; i<numScratch; i++)
    scratch[i] = m_uart.readByte();

  // resolution is stored in byte 4
  scratch[4] = ((scratch[4] & ~(_CFG_RESOLUTION_MASK << _CFG_RESOLUTION_SHIFT))
                | (res << _CFG_RESOLUTION_SHIFT));

  // now, write back, we only write 3 bytes (2-4), no cksum.
  m_uart.command(CMD_WRITE_SCRATCHPAD, m_deviceMap[index].id);
  for (int i=0; i<3; i++)
    m_uart.writeByte(scratch[i+2]);
}

void DS18B20::copyScratchPad(int index)
{
  if (index < 0 || index >= m_devicesFound)
    {
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": device index out of range");
    }

  // issue the command
  m_uart.command(CMD_COPY_SCRATCHPAD, m_deviceMap[index].id);

  sleep(1); // to be safe...
}

void DS18B20::recallEEPROM(int index)
{
  if (index < 0 || index >= m_devicesFound)
    {
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": device index out of range");
    }

  // issue the command
  m_uart.command(CMD_RECALL_EEPROM, m_deviceMap[index].id);

  // issue read timeslots until a '1' is read back, indicating completion
  while (!m_uart.writeBit(1))
    usleep(100);
}
