/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Adapted from Seeed Studio library:
 * https://github.com/Seeed-Studio/RTC_DS1307
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

#include "ds1307.h"

using namespace upm;
using namespace std;


DS1307::DS1307(int bus) : m_i2c(bus)
{
  // setup our i2c link
  mraa::Result ret = m_i2c.address(DS1307_I2C_ADDR);
  if (ret != mraa::SUCCESS){
    throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": i2c.address() failed");
    return;
  }
}

mraa::Result DS1307::writeBytes(uint8_t reg, uint8_t *buffer, int len)
{
  if (!len || !buffer)
    return mraa::ERROR_INVALID_PARAMETER;

  // create a buffer 1 byte larger than the supplied buffer,
  // store the register in the first byte
  uint8_t buf2[len + 1];

  buf2[0] = reg;

  // copy in the buffer after the reg byte
  for (int i=1; i<(len + 1); i++)
    buf2[i] = buffer[i-1];

  mraa::Result ret = m_i2c.address(DS1307_I2C_ADDR);
  if (ret != mraa::SUCCESS){
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": i2c.address() failed");
      return ret;
  }

  return m_i2c.write(buf2, len + 1);
}

int DS1307::readBytes(uint8_t reg, uint8_t *buffer, int len)
{
  if (!len || !buffer)
    return 0;

  mraa::Result ret = m_i2c.address(DS1307_I2C_ADDR);
  if (ret != mraa::SUCCESS){
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": i2c.address() failed");
      return 0;
  }
  m_i2c.writeByte(reg);

  return m_i2c.read(buffer, len);
}

bool DS1307::loadTime()
{
  // read the first 7 registers
  uint8_t buffer[7];
  int bytesRead = readBytes(0, buffer, 7);

  if (bytesRead != 7)
    { 
      // problem
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": failed to read expected 7 bytes from device");
      return false;
    }

  // We need to mask some control bits off of some of these values
  // and convert the result to decimal from BCD.  We also need to account
  // for format (AM/PM or 24hr), and if AM/PM, whether PM should be set.
  
  // first bit here is the oscillator enable/disable bit
  seconds = bcdToDec(buffer[0] & 0x7f);
  minutes = bcdToDec(buffer[1]);

  // check AM/PM or 24hr mode
  if (buffer[2] & 0x40)
    {
      // We are in AM/PM mode
      hours = bcdToDec(buffer[2] & 0x1f);
      amPmMode = true;
      pm = (buffer[2] & 0x20) ? true : false;
    }
  else
    {
      // 24hr mode
      hours = bcdToDec(buffer[2] & 0x3f);
      amPmMode = false;
      pm = false;
    }

  dayOfWeek = bcdToDec(buffer[3]);
  dayOfMonth = bcdToDec(buffer[4]);
  month = bcdToDec(buffer[5]);
  year = bcdToDec(buffer[6]);

  return true;
}

bool DS1307::setTime()
{
  uint8_t buffer[7];

  // seconds
  // we need to read in seconds first to preserve the osc enable bit
  uint8_t tmpbuf;

  readBytes(0, &tmpbuf, 1);
  buffer[0] = decToBcd(seconds) | (tmpbuf & 0x80);

  // minutes 
  buffer[1] = decToBcd(minutes);

  // hours
  if (amPmMode)
    {
      buffer[2] = decToBcd(hours) | 0x40;
      if (pm)
        buffer[2] |= 0x20;
    }
  else
    buffer[2] = decToBcd(hours);

  // day of week
  buffer[3] = decToBcd(dayOfWeek);

  // day of month
  buffer[4] = decToBcd(dayOfMonth);

  // month
  buffer[5] = decToBcd(month);

  // year
  buffer[6] = decToBcd(year);

  return writeBytes(0, buffer, 7);
}

mraa::Result DS1307::enableClock()
{
  // the oscillator enable bit is the high bit of reg 0
  // so read it, clear it, and write it back.

  uint8_t buf;
  readBytes(0, &buf, 1);

  buf &= ~0x80;

  return writeBytes(0, &buf, 1);
}

mraa::Result DS1307::disableClock()
{
  // the oscillator enable bit is the high bit of reg 0
  // so read it, set it, and write it back.

  uint8_t buf;
  readBytes(0, &buf, 1);

  buf |= 0x80;

  return writeBytes(0, &buf, 1);
}


// Convert decimal to BCD
uint8_t DS1307::decToBcd(unsigned int val)
{
  return ( (val/10*16) + (val%10) );
}

// Convert BCD to decimal
unsigned int DS1307::bcdToDec(uint8_t val)
{
  return ( (val/16*10) + (val%16) );
}

