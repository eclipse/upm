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

#include "sht1x.hpp"

using namespace upm;
using namespace std;

SHT1X::SHT1X(unsigned int clk_pin, unsigned int data_pin) :
  m_sht1x(sht1x_init(clk_pin, data_pin))
{
  if (!m_sht1x)
    throw std::runtime_error(string(__FUNCTION__)
                             + ": sht1x_init() failed");
}

SHT1X::~SHT1X()
{
  sht1x_close(m_sht1x);
}

void SHT1X::reset()
{
  sht1x_reset(m_sht1x);
}

void SHT1X::update()
{
  if (sht1x_update(m_sht1x))
    throw std::runtime_error(string(__FUNCTION__)
                             + ": sht1x_update() failed");
}

float SHT1X::getTemperature()
{
  return sht1x_get_temperature(m_sht1x);
}

float SHT1X::getHumidity()
{
  return sht1x_get_humidity(m_sht1x);
}

uint8_t SHT1X::readStatus()
{
  uint8_t status;

  if (sht1x_read_status(m_sht1x, &status))
    throw std::runtime_error(string(__FUNCTION__)
                             + ": sht1x_read_status() failed");

  return status;
}

upm_result_t SHT1X::writeStatus(uint8_t status)
{
  return sht1x_write_status(m_sht1x, status);
}

void SHT1X::setVolts(SHT1X_VOLTS_T volts)
{
  sht1x_set_volts(m_sht1x, volts);
}

upm_result_t SHT1X::sendCommand(SHT1X_CMD_T cmd)
{
  return sht1x_send_command(m_sht1x, cmd);
}

upm_result_t SHT1X::waitForResponse()
{
  return sht1x_wait_for_response(m_sht1x);
}

void SHT1X::startXmit()
{
  sht1x_start_xmit(m_sht1x);
}

uint8_t SHT1X::read8Bits()
{
  uint8_t value;

  sht1x_read_8bits(m_sht1x, &value);

  return value;
}

upm_result_t SHT1X::write8Bits(uint8_t byte)
{
  return sht1x_write_8bits(m_sht1x, byte);
}
