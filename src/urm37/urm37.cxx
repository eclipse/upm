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

#include "urm37.h"

using namespace upm;
using namespace std;

static const int waitTimeout = 1000;
static const int maxRetries = 10;

URM37::URM37(int aPin, int resetPin, int triggerPin, float aref) :
  m_uart(0), m_aio(new mraa::Aio(aPin)), m_gpioReset(resetPin), 
  m_gpioTrigger(new mraa::Gpio(triggerPin))
{
  m_analogMode = true;

  m_aRes = (1 << m_aio->getBit());
  m_aref = aref;

  m_gpioTrigger->dir(mraa::DIR_OUT);

  // setup trigger for mmapped access, not a big deal if this fails
  m_gpioTrigger->useMmap(true);

  // trigger high
  m_gpioTrigger->write(1);

  init();
}

URM37::URM37(int uart, int resetPin) :
  m_uart(new mraa::Uart(uart)), m_aio(0), m_gpioReset(resetPin), 
  m_gpioTrigger(0)
{
  m_analogMode = false;

  m_aRes = 0;
  m_aref = 0;

  // 9600 baud is the only support baud rate...
  if (m_uart->setBaudRate(9600))
    {
      throw std::runtime_error(string(__FUNCTION__) +
                               ": setBaudRate(9600) failed");
      return;
    }

  init();
}

URM37::~URM37()
{
  if (m_uart)
    delete m_uart;
  if (m_aio)
    delete m_aio;
  if(m_gpioTrigger)
    delete m_gpioTrigger;
}

void URM37::init()
{
  m_gpioReset.dir(mraa::DIR_OUT);

  // reset the device
  reset();
}

void URM37::reset()
{
  // toggle reset
  m_gpioReset.write(0);
  usleep(100);
  m_gpioReset.write(1);
  // wait for reset to complete
  sleep(3);
}

bool URM37::dataAvailable(unsigned int millis)
{
  return m_uart->dataAvailable(millis);
}

std::string URM37::readDataStr(int len)
{
  return m_uart->readStr(len);
}

int URM37::writeDataStr(std::string data)
{
  m_uart->flush();
  return m_uart->writeStr(data);
}

float URM37::getDistance(int degrees)
{
  // analog mode
  if (m_analogMode)
    {
      m_gpioTrigger->write(0);
      int val = m_aio->read();
      m_gpioTrigger->write(1);
      
      float mVolts = (float(val) * (m_aref / m_aRes)) * 1000.0;
      
      // 6.8mV per CM
      return (mVolts / 6.8);
    }

  // UART mode
  // query distance cmd sequence
  uint8_t deg = (uint8_t)(degrees / 6);
  if (deg > 46)
    throw std::out_of_range(string(__FUNCTION__) +
                            ": degrees out of range, must be 0-270");

  string cmd;
  uint8_t cksum = 0x22 + deg + 0x00;
  cmd.push_back(0x22);
  cmd.push_back(deg);
  cmd.push_back(0x00);
  cmd.push_back(cksum);
  
  string resp = sendCommand(cmd);

  if (resp.empty())
    {
      throw std::runtime_error(string(__FUNCTION__) +
                               ": sendCommand() failed");
      return 0.0;
    }

  uint8_t h = (uint8_t)resp[1];
  uint8_t l = (uint8_t)resp[2];

  float distance = float((h << 8) | l);

  return (distance);
}

float URM37::getTemperature()
{
  if (m_analogMode)
    {
      throw std::runtime_error(string(__FUNCTION__) +
                 ": Temperature measurement not available in analog mode");

      return 0.0;
    }

  // query temperature cmd sequence
  string cmd;
  cmd.push_back(0x11);
  cmd.push_back(0x00);
  cmd.push_back(0x00);
  cmd.push_back(0x11);
  
  string resp = sendCommand(cmd);

  if (resp.empty())
    {
      throw std::runtime_error(string(__FUNCTION__) +
                               ": sendCommand() failed");
      return 0.0;
    }

  uint8_t h = (uint8_t)resp[1];
  uint8_t l = (uint8_t)resp[2];

  float temp;
  temp = float((h & 0x0f) * 256 + l) / 10.0;
  if (h & 0xf0)
    temp *= -1;

  return (temp);
}

uint8_t URM37::readEEPROM(uint8_t addr)
{
  if (m_analogMode)
    {
      throw std::runtime_error(string(__FUNCTION__) +
                 ": readEEPROM() is not possible in analog mode");

      return 0;
    }

  if (addr > 0x04)
    throw std::out_of_range(string(__FUNCTION__) +
                            ": addr must be between 0x00-0x04");

  string cmd;
  uint8_t cksum = 0x33 + addr + 0x00;
  cmd.push_back(0x33);
  cmd.push_back(addr);
  cmd.push_back(0x00);
  cmd.push_back(cksum);

  string resp = sendCommand(cmd);

  if (resp.empty())
    {
      throw std::runtime_error(string(__FUNCTION__) +
                               ": sendCommand() failed");
      return 0;
    }

  return resp[2];
}

void URM37::writeEEPROM(uint8_t addr, uint8_t value)
{
  if (m_analogMode)
    {
      throw std::runtime_error(string(__FUNCTION__) +
                 ": writeEEPROM() is not possible in analog mode");

      return;
    }

  if (addr > 0x04)
    throw std::out_of_range(string(__FUNCTION__) +
                            ": addr must be between 0x00-0x04");

  string cmd;
  uint8_t cksum = 0x44 + addr + value;
  cmd.push_back(0x44);
  cmd.push_back(addr);
  cmd.push_back(value);
  cmd.push_back(cksum);

  string resp = sendCommand(cmd);

  if (resp.empty())
    {
      throw std::runtime_error(string(__FUNCTION__) +
                               ": sendCommand() failed");
      return;
    }

  return;
}

string URM37::sendCommand(string cmd)
{
  if (m_analogMode)
    {
      throw std::runtime_error(string(__FUNCTION__) +
                               ": can only be executed in UART mode");

      return "";
    }

  int tries = 0;
  string resp;

  while (tries++ < maxRetries)
    {
      writeDataStr(cmd);
      if (!dataAvailable(waitTimeout))
        {
          cerr << __FUNCTION__ << ": Timed out waiting for response" << endl;
          continue;
        }
      
      resp = readDataStr(8);
      
      // verify size
      if (resp.size() != 4)
        {
          cerr << __FUNCTION__ << ": Invalid returned packet size" << endl;
          continue;
        }
      else
        {
          // we have data, verify cksum, return the response if it's
          // good, retry otherwise
          uint8_t cksum = (uint8_t)(resp[0] + resp[1] + resp[2]);

          if ((uint8_t)resp[3] != cksum)
            {
              cerr << __FUNCTION__ << ": cksum failure" << endl;
              continue;
            }

          // else, we are good to go
          return resp;
        }
    }

  // :(
  return "";
}

