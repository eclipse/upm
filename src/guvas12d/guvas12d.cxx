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

#include "guvas12d.hpp"

using namespace upm;
using namespace std;

GUVAS12D::GUVAS12D(int pin, float aref) :
    m_guvas12d(guvas12d_init(pin, aref))
{
    if (!m_guvas12d)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": guvas12d_init() failed");
      return;
    }
}

GUVAS12D::~GUVAS12D()
{
    guvas12d_close(m_guvas12d);
}

float GUVAS12D::value(float aref, unsigned int samples)
{
    (void)(samples); // unused, this method is deprecated.

    // this is a hack, but this function should go away anyway
    if (aref != m_guvas12d->aref)
        m_guvas12d->aref = aref;

    return volts();
}

float GUVAS12D::volts()
{
    float volts;
    if (guvas12d_get_volts(m_guvas12d, &volts))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": guvas12d_get_volts() failed");
    }

    return volts;
}

float GUVAS12D::intensity()
{
    float i;
    if (guvas12d_get_intensity(m_guvas12d, &i))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": guvas12d_get_intensity() failed");
    }

    return i;
}

void GUVAS12D::setScale(float scale)
{
    guvas12d_set_scale(m_guvas12d, scale);
}

void GUVAS12D::setOffset(float offset)
{
    guvas12d_set_offset(m_guvas12d, offset);
}
