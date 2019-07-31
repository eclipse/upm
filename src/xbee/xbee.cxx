/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <time.h>

#include "xbee.hpp"

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
  for (size_t i=0; i<str.size(); i++)
    if (str[i] == '\r')
      str[i] = '\n';
  
  return str;
}
