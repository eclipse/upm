/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014-2016 Intel Corporation.
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
