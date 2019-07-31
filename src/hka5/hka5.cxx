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
