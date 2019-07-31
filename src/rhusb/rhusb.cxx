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

float RHUSB::getTemperature()
{
  return getTemperature(false);
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
    }

  // Terminate the command with a carriage return
  writeStr(cmd + "\r");

  string resp;
  // I see random timeouts with wait values below 250ms
  while (dataAvailable(250) && (resp.size() < maxBuffer) )
    {
      resp += readStr(maxBuffer);
    }

  if (resp.empty())
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": " + cmd + " response timed out");
    }

  // check that the last character is the prompt
  if (resp.at(resp.size() - 1) != '>')
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": " + cmd + " response corrupt");
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
