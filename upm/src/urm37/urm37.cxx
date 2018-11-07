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
#include <stdexcept>

#include "urm37.hpp"

using namespace upm;
using namespace std;

URM37::URM37(int aPin, int resetPin, int triggerPin, float aref) :
  m_urm37(urm37_init(aPin, resetPin, triggerPin, aref, 0, true))
{
  if (!m_urm37)
    throw std::runtime_error(string(__FUNCTION__) +
                             ": urm37_init() failed");
}

URM37::URM37(int uart, int resetPin) :
  m_urm37(urm37_init(0, resetPin, 0, 0, uart, false))
{
  if (!m_urm37)
    throw std::runtime_error(string(__FUNCTION__) +
                             ": urm37_init() failed");
}

URM37::~URM37()
{
  urm37_close(m_urm37);
}

void URM37::reset()
{
  urm37_reset(m_urm37);
}

float URM37::getDistance(int degrees)
{
  float distance;

  if (urm37_get_distance(m_urm37, &distance, degrees) != UPM_SUCCESS)
    throw std::runtime_error(string(__FUNCTION__) +
                             ": urm37_get_distance() failed");

  return (distance);
}

float URM37::getTemperature()
{
  float temperature;

  if (urm37_get_temperature(m_urm37, &temperature) != UPM_SUCCESS)
    throw std::runtime_error(string(__FUNCTION__) +
                             ": urm37_get_temperature() failed");

  return temperature;
}

uint8_t URM37::readEEPROM(uint8_t addr)
{
  uint8_t value;

  if (urm37_read_eeprom(m_urm37, addr, &value) != UPM_SUCCESS)
    throw std::runtime_error(string(__FUNCTION__) +
                             ": urm37_read_eeprom() failed");

  return value;
}

void URM37::writeEEPROM(uint8_t addr, uint8_t value)
{
  if (urm37_write_eeprom(m_urm37, addr, value) != UPM_SUCCESS)
    throw std::runtime_error(string(__FUNCTION__) +
                             ": urm37_write_eeprom() failed");
}

