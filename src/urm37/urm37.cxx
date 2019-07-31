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

float URM37::getDistance()
{
    /* TODO: compilation issue for swig. switched original method to not use default zero parameter. */
    return getDistance(0);
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

