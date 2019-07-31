/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014-2016 Intel Corporation.
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

#include "linefinder.hpp"

using namespace upm;
using namespace std;

LineFinder::LineFinder(int pin) :
    m_linefinder(linefinder_init(pin))
{
  if (!m_linefinder)
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": linefinder_init() failed");
}

LineFinder::~LineFinder()
{
    linefinder_close(m_linefinder);
}

bool LineFinder::whiteDetected()
{
    return linefinder_white_detected(m_linefinder);
}

bool LineFinder::blackDetected()
{
    return linefinder_black_detected(m_linefinder);
}
