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
#include <stdexcept>

#include "nmea_gps.hpp"

using namespace upm;
using namespace std;

NMEAGPS::NMEAGPS(unsigned int uart, unsigned int baudrate,
                 int enable_pin) :
  m_nmea_gps(nmea_gps_init(uart, baudrate, enable_pin))
{
  if (!m_nmea_gps)
    throw std::runtime_error(string(__FUNCTION__)
                             + ": nmea_gps_init() failed");
}

NMEAGPS::NMEAGPS(unsigned int bus, uint8_t addr) :
  m_nmea_gps(nmea_gps_init_ublox_i2c(bus, addr))
{
  if (!m_nmea_gps)
    throw std::runtime_error(string(__FUNCTION__)
                             + ": nmea_gps_init() failed");
}

NMEAGPS::~NMEAGPS()
{
  nmea_gps_close(m_nmea_gps);
}

std::string NMEAGPS::readStr(size_t size)
{
  char buffer[size];

  int rv;

  if ((rv = nmea_gps_read(m_nmea_gps, buffer, size)) < 0)
    throw std::runtime_error(string(__FUNCTION__)
                             + ": nmea_gps_read() failed");

  return string(buffer, rv);
}

int NMEAGPS::writeStr(std::string buffer)
{
  int rv;

  if ((rv = nmea_gps_write(m_nmea_gps, (char*)buffer.data(),
                           buffer.size())) < 0)
    throw std::runtime_error(string(__FUNCTION__)
                             + ": nmea_gps_write() failed");

  return rv;  
}

void NMEAGPS::enable(bool enable)
{
  if (nmea_gps_enable(m_nmea_gps, enable))
    throw std::runtime_error(string(__FUNCTION__)
                             + ": nmea_gps_enable() failed");
}

void NMEAGPS::setBaudrate(unsigned int baudrate)
{
  if (nmea_gps_set_baudrate(m_nmea_gps, baudrate))
    throw std::runtime_error(string(__FUNCTION__)
                             + ": nmea_gps_baudrate() failed");
}

bool NMEAGPS::dataAvailable(unsigned int millis)
{
  return nmea_gps_data_available(m_nmea_gps, millis);
}
