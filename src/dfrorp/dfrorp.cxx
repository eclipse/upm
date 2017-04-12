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

#include "dfrorp.hpp"

using namespace upm;
using namespace std;

DFRORP::DFRORP(int apin, float a_ref) :
  m_dfrorp(dfrorp_init(apin, a_ref))
{
  if (!m_dfrorp)
    throw std::runtime_error(string(__FUNCTION__)
                             + ": dfrorp_init() failed");
}

DFRORP::~DFRORP()
{
  dfrorp_close(m_dfrorp);
}

void DFRORP::update()
{
  upm_result_t rv;

  if ((rv = dfrorp_update(m_dfrorp)))
    {
      throw std::runtime_error(string(__FUNCTION__)
                               + ": dfrorp_update() failed with UPM error "
                               + std::to_string(int(rv)) );
    }
}

void DFRORP::setOffset(float offset)
{
  dfrorp_set_offset(m_dfrorp, offset);
}

void DFRORP::setScale(float scale)
{
  dfrorp_set_scale(m_dfrorp, scale);
}

void DFRORP::setCalibrationOffset(float offset)
{
  dfrorp_set_calibration_offset(m_dfrorp, offset);
}

float DFRORP::getORP()
{
  return dfrorp_get_orp(m_dfrorp);
}

float DFRORP::getVolts()
{
  return dfrorp_get_volts(m_dfrorp);
}

float DFRORP::getNormalized()
{
  return dfrorp_get_normalized(m_dfrorp);
}
