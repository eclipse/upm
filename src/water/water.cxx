/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <string>
#include <stdexcept>

#include "water.hpp"

using namespace upm;
using namespace std;

Water::Water(unsigned int pin) :
  m_water(water_init(pin))
{
  if (!m_water )
    throw std::runtime_error(std::string(__FUNCTION__) +
                             ": water_init() failed");
}

Water::~Water()
{
  water_close(m_water);
}

bool Water::isWet()
{
  return water_is_wet(m_water);
}
