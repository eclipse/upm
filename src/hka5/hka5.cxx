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

#include "hka5.hpp"

using namespace upm;
using namespace std;

HKA5::HKA5(unsigned int uart, int set_pin, int reset_pin) :
  m_hka5(hka5_init(uart, set_pin, reset_pin))
{
  if (!m_hka5)
    throw std::runtime_error(string(__FUNCTION__)
                             + ": hka5_init() failed");
}

HKA5::~HKA5()
{
  hka5_close(m_hka5);
}

void HKA5::enable(bool enable)
{
  if (hka5_enable(m_hka5, enable))
    throw std::runtime_error(string(__FUNCTION__)
                             + ": hka5_enable() failed");
}

void HKA5::reset()
{
  if (hka5_reset(m_hka5))
    throw std::runtime_error(string(__FUNCTION__)
                             + ": hka5_reset() failed");
}

void HKA5::update()
{
  if (hka5_update(m_hka5))
    throw std::runtime_error(string(__FUNCTION__)
                             + ": hka5_update() failed");
}

unsigned int HKA5::getPM1()
{
  return hka5_get_pm1(m_hka5);
}

unsigned int HKA5::getPM2_5()
{
  return hka5_get_pm2_5(m_hka5);
}

unsigned int HKA5::getPM10()
{
  return hka5_get_pm10(m_hka5);
}
