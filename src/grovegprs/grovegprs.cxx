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

#include "grovegprs.h"

using namespace upm;
using namespace std;

static const int defaultDelay = 100;     // max wait time for read

GroveGPRS::GroveGPRS(int uart) :
  m_uart(uart)
{
}

GroveGPRS::~GroveGPRS()
{
}

bool GroveGPRS::dataAvailable(unsigned int millis)
{
  return m_uart.dataAvailable(millis);
}

int GroveGPRS::readData(char *buffer, unsigned int len)
{
  return m_uart.read(buffer, len);
}

std::string GroveGPRS::readDataStr(int len)
{
  return m_uart.readStr(len);
}

int GroveGPRS::writeData(char *buffer, unsigned int len)
{
  m_uart.flush();
  return m_uart.write(buffer, len);
}

int GroveGPRS::writeDataStr(std::string data)
{
  m_uart.flush();
  return m_uart.writeStr(data);
}

mraa::Result GroveGPRS::setBaudRate(int baud)
{
  return m_uart.setBaudRate(baud);
}

