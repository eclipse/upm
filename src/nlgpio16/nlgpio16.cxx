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

#include "nlgpio16.hpp"

using namespace upm;
using namespace std;

static const int maxBuffer = 1024;
static const int baudRate = 115200;

static char num2Hex(int i, bool upcase)
{
  char temp;

  switch (i)
    {
    case 0: return '0';
    case 1: return '1';
    case 2: return '2';
    case 3: return '3';
    case 4: return '4';
    case 5: return '5';
    case 6: return '6';
    case 7: return '7';
    case 8: return '8';
    case 9: return '9';
    case 10: temp = (upcase) ? 'A' : 'a'; return temp;
    case 11: temp = (upcase) ? 'B' : 'b'; return temp;
    case 12: temp = (upcase) ? 'C' : 'c'; return temp;
    case 13: temp = (upcase) ? 'D' : 'd'; return temp;
    case 14: temp = (upcase) ? 'E' : 'e'; return temp;
    case 15: temp = (upcase) ? 'F' : 'f'; return temp;

    default:
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": number must be between 0 and 15");
      return '0';
    }
}

NLGPIO16::NLGPIO16(string uart) :
  m_uart(uart)
{
  m_uart.setBaudRate(baudRate);
}

NLGPIO16::~NLGPIO16()
{
}

bool NLGPIO16::dataAvailable(unsigned int millis)
{
  return m_uart.dataAvailable(millis);
}

std::string NLGPIO16::readStr(int len)
{
  return m_uart.readStr(len);
}

int NLGPIO16::writeStr(std::string data)
{
  m_uart.flush();
  return m_uart.writeStr(data);
}

string NLGPIO16::sendCommand(string cmd)
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
  while (dataAvailable(10))
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

  // delete the last 3 characters, which should be '\n\r>'
  resp.erase(resp.size() - 3, 3);

  // find first delimeter (second should already be erased)
  size_t pos = resp.find("\n\r");

  // if we didn't find one, then the command was invalid.  This
  // shouldn't happen, but we check for it anyway
  if (pos == string::npos)
    return "";

  pos += 2;                     // skip past them.

  // now we should have the value (or an empty string in the case of a
  // write command).  Erase everything else and return the result.
  resp.erase(0, pos);

  return resp;
}

void NLGPIO16::gpioSet(int gpio)
{
  if (gpio < 0 || gpio > 15)
    {
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": gpio must be between 0 and 15");
      return;
    }

  string cmd("gpio set ");
  cmd += num2Hex(gpio, true);

  // set commands don't return anything useful
  sendCommand(cmd);
  return;
}

void NLGPIO16::gpioClear(int gpio)
{
  if (gpio < 0 || gpio > 15)
    {
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": gpio must be between 0 and 15");
      return;
    }

  string cmd("gpio clear ");
  cmd += num2Hex(gpio, true);

  // set commands don't return anything useful
  sendCommand(cmd);
  return;
}

bool NLGPIO16::gpioRead(int gpio)
{
  if (gpio < 0 || gpio > 15)
    {
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": gpio must be between 0 and 15");
      return false;
    }

  string cmd("gpio read ");
  cmd += num2Hex(gpio, true);

  string resp = sendCommand(cmd);
  if (resp.empty())
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": invalid empty response from device");
      return false;
    }

  // c++11 toString() would be handy...
  return ( (atoi(resp.c_str()) == 0) ? false : true );
}

unsigned int NLGPIO16::gpioReadAll()
{
  string cmd("gpio readall");

  string resp = sendCommand(cmd);
  if (resp.empty())
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": invalid empty response from device");
      return false;
    }

  //  Convert to unsigned int (but mask out everything but the lower 16bits)
  unsigned int value = strtoul(resp.c_str(), NULL, 16);
  value &= 0xffff;

  return value;
}

void NLGPIO16::gpioSetIOMask(int mask)
{
  if (mask < 0 || mask > 65535)
    {
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": mask must be between 0 and 65535");
      return;
    }

  string cmd("gpio iomask ");
  cmd += num2Hex( ((mask & 0xf000) >> 12), false);
  cmd += num2Hex( ((mask & 0x0f00) >> 8), false);
  cmd += num2Hex( ((mask & 0x00f0) >> 4), false);
  cmd += num2Hex( ((mask & 0x000f) >> 0), false);

  sendCommand(cmd);

  return;
}

void NLGPIO16::gpioSetIODir(int mask)
{
  if (mask < 0 || mask > 65535)
    {
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": mask must be between 0 and 65535");
      return;
    }

  string cmd("gpio iodir ");
  cmd += num2Hex( ((mask & 0xf000) >> 12), false);
  cmd += num2Hex( ((mask & 0x0f00) >> 8), false);
  cmd += num2Hex( ((mask & 0x00f0) >> 4), false);
  cmd += num2Hex( ((mask & 0x000f) >> 0), false);

  sendCommand(cmd);

  return;
}

void NLGPIO16::gpioWriteAll(int mask)
{
  if (mask < 0 || mask > 65535)
    {
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": mask must be between 0 and 65535");
      return;
    }

  string cmd("gpio writeall ");
  cmd += num2Hex( ((mask & 0xf000) >> 12), false);
  cmd += num2Hex( ((mask & 0x0f00) >> 8), false);
  cmd += num2Hex( ((mask & 0x00f0) >> 4), false);
  cmd += num2Hex( ((mask & 0x000f) >> 0), false);

  sendCommand(cmd);

  return;
}

string NLGPIO16::getVersion()
{
  return sendCommand("vers");
}

string NLGPIO16::getID()
{
  return sendCommand("id get");
}

void NLGPIO16::setID(string id)
{
  if (id.size() != 8)
    {
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": ID must be exactly 8 characters in length");
      return;
    }

  string cmd("id set ");
  cmd += id;

  sendCommand(cmd);

  return;
}

int NLGPIO16::analogReadValue(int adc)
{
  // Only ports 0-6 are ADC capable
  if (adc < 0 || adc > 6)
    {
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": adc must be between 0 and 6");
      return false;
    }

  string cmd("adc read ");
  cmd += num2Hex(adc, true);

  string resp = sendCommand(cmd);
  if (resp.empty())
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": invalid empty response from device");
      return false;
    }

  // c++11 stoi() would be handy...
  return atoi(resp.c_str());
}

float NLGPIO16::analogReadVolts(int adc)
{
  float value = float(analogReadValue(adc));

  return ( value * (ADC_AREF / float(1 << ADC_PRECISION)) );
}

