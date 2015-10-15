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

#include <iostream>
#include <time.h>

#include "xbee.h"

using namespace upm;
using namespace std;

static const int maxBuffer = 1024;

XBee::XBee(int uart) :
  m_uart(uart)
{
}

XBee::~XBee()
{
}

bool XBee::dataAvailable(unsigned int millis)
{
  return m_uart.dataAvailable(millis);
}

int XBee::readData(char *buffer, unsigned int len)
{
  return m_uart.read(buffer, len);
}

std::string XBee::readDataStr(int len)
{
  return m_uart.readStr(len);
}

int XBee::writeData(char *buffer, unsigned int len)
{
  m_uart.flush();
  return m_uart.write(buffer, len);
}

int XBee::writeDataStr(std::string data)
{
  m_uart.flush();
  return m_uart.writeStr(data);
}

mraa::Result XBee::setBaudRate(int baud)
{
  return m_uart.setBaudRate(baud);
}

bool XBee::commandMode(std::string cmdChars, int guardTimeMS)
{

  usleep(guardTimeMS * 1000);

  writeDataStr(cmdChars);

  usleep(guardTimeMS * 1000);

  string resp;
  if (dataAvailable(1000))
    resp = readDataStr(maxBuffer);

  if (resp.find("OK"))
    return true;
  else
    return false;
}

string XBee::stringCR2LF(string str)
{
  for (int i=0; i<str.size(); i++)
    if (str[i] == '\r')
      str[i] = '\n';
  
  return str;
}
