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

#include "loudness.hpp"

using namespace std;
using namespace upm;

Loudness::Loudness(int pin, float aref) :
  m_aio(pin)
{
  m_aRes = m_aio.getBit();
  m_aref = aref;
}

Loudness::~Loudness()
{
}

float Loudness::loudness()
{
  int val = m_aio.read();

  return(val * (m_aref / float(1 << m_aRes)));
}
