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

#include "gprs.hpp"

using namespace upm;
using namespace std;

GPRS::GPRS(int uart) :
  m_uart(uart)
{
}

GPRS::~GPRS()
{
}

bool GPRS::dataAvailable(unsigned int millis)
{
  return m_uart.dataAvailable(millis);
}

int GPRS::readData(char *buffer, unsigned int len)
{
  return m_uart.read(buffer, len);
}

std::string GPRS::readDataStr(int len)
{
  return m_uart.readStr(len);
}

int GPRS::writeData(char *buffer, unsigned int len)
{
  m_uart.flush();
  return m_uart.write(buffer, len);
}

int GPRS::writeDataStr(std::string data)
{
  m_uart.flush();
  return m_uart.writeStr(data);
}

int GPRS::setBaudRate(int baud)
{
  int ret_code = m_uart.setBaudRate(baud);
  if (ret_code != MRAA_SUCCESS) {
    return ret_code;
  } else {
    return 0;
  }
  //return m_uart.setBaudRate(baud);
}

