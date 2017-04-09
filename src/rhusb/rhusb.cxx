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
#include <stdlib.h>
#include <iostream>
#include <stdexcept>
#include <string>

#include "rhusb.hpp"

using namespace upm;
using namespace std;

static const int maxBuffer = 1024;
// baud rate is always 9600
static const int baudRate = 9600;

// conversion from celsius to fahrenheit

static float c2f(float c)
{
  return (c * (9.0 / 5.0) + 32.0);
}

RHUSB::RHUSB(std::string device) :
  m_uart(device)
{
  m_uart.setBaudRate(baudRate);

  m_temperature = 0.0;
  m_humidity = 0.0;
}

RHUSB::~RHUSB()
{
}

void RHUSB::update()
{
  char *endptr;
  float value;

  // first get the temp in C
  string resp = sendCommand("C");

  // convert
  value = strtof(resp.c_str(), &endptr);

  // check for conversion failure
  if (value == 0.0 && resp.c_str() == endptr)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": strtof() temperature conversion failed");
    }
  m_temperature = value;

  // now humidity
  resp = sendCommand("H");

  // convert
  value = strtof(resp.c_str(), &endptr);

  // check for conversion failure
  if (value == 0.0 && resp.c_str() == endptr)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": strtof() humidity conversion failed");
    }
  m_humidity = value;
}

float RHUSB::getTemperature(bool fahrenheit)
{
  if (fahrenheit)
    return c2f(m_temperature);
  else
    return m_temperature;
}

float RHUSB::getHumidity()
{
  return m_humidity;
}

bool RHUSB::dataAvailable(unsigned int millis)
{
  return m_uart.dataAvailable(millis);
}

string RHUSB::readStr(int len)
{
  return m_uart.readStr(len);
}

int RHUSB::writeStr(string data)
{
  m_uart.flush();
  return m_uart.writeStr(data);
}

string RHUSB::sendCommand(string cmd)
{
  // make sure we got a command
  if (cmd.empty())
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": cmd is empty!");
      return "";
    }

  // make sure string is CR terminated
  if (cmd.at(cmd.size() - 1) != '\r')
    cmd.append("\r");

  writeStr(cmd);

  string resp;
  // I see random timeouts with wait values below 250ms
  while (dataAvailable(250))
    {
      resp += readStr(maxBuffer);
    }

  if (resp.empty())
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": timed out waiting for response");
      return "";
    }

  // check that the last character is the prompt
  if (resp.at(resp.size() - 1) != '>')
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": read from device corrupted");
      return "";
    }

  // delete the last 3 characters, which should be '\r\n>'
  resp.erase(resp.size() - 3, 3);

  return resp;
}

string RHUSB::getFirmwareID()
{
  string resp = sendCommand("ENQ");

  // For readability, replace the intermediate \r\n with a space if found.

  size_t pos = resp.find("\r\n");

  if (pos != string::npos)
    resp.replace(pos, 2, " ");

  return resp;
}
