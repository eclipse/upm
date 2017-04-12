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

#include "bh1750.hpp"

using namespace upm;
using namespace std;

BH1750::BH1750(int bus, int addr, BH1750_OPMODES_T mode) :
  m_bh1750(bh1750_init(bus, addr, mode))
{
  if (!m_bh1750)
    throw std::runtime_error(string(__FUNCTION__) +
                             ": bh1750_init() failed");
}

BH1750::~BH1750()
{
  bh1750_close(m_bh1750);
}

void BH1750::reset()
{
  bh1750_reset(m_bh1750);
}

float BH1750::getLux()
{
  float lux = 0.0;

  if (bh1750_get_lux(m_bh1750, &lux) != UPM_SUCCESS)
    throw std::runtime_error(string(__FUNCTION__) +
                             ": bh1750_get_lux() failed");

  return lux;
}

void BH1750::powerUp()
{
  if (bh1750_power_up(m_bh1750) != UPM_SUCCESS)
    throw std::runtime_error(string(__FUNCTION__) +
                             ": bh1750_power_up() failed");
}

void BH1750::powerDown()
{
  if (bh1750_power_down(m_bh1750) != UPM_SUCCESS)
    throw std::runtime_error(string(__FUNCTION__) +
                             ": bh1750_power_down() failed");
}

void BH1750::setOpmode(BH1750_OPMODES_T mode)
{
  if (bh1750_set_opmode(m_bh1750, mode) != UPM_SUCCESS)
    throw std::runtime_error(string(__FUNCTION__) +
                             ": bh1750_set_opmode() failed");
}

void BH1750::sendCommand(uint8_t cmd)
{
  if (bh1750_send_command(m_bh1750, cmd) != UPM_SUCCESS)
    throw std::runtime_error(string(__FUNCTION__) +
                             ": bh1750_send_command() failed");
}

uint16_t BH1750::readData()
{
  uint16_t data = 0;

  if (bh1750_read_data(m_bh1750, &data) != UPM_SUCCESS)
    throw std::runtime_error(string(__FUNCTION__) +
                             ": bh1750_read_data() failed");

  return data;
}
