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
#include <string>
#include <stdexcept>

#include "dfrec.hpp"

using namespace upm;
using namespace std;

DFREC::DFREC(unsigned int apin, unsigned int uart_ow,
             unsigned int device_idx,
             float a_ref) :
  m_dfrec(dfrec_init(apin, uart_ow, device_idx, a_ref))
{
  if (!m_dfrec)
    throw std::runtime_error(string(__FUNCTION__)
                             + ": dfrec_init() failed");
}

DFREC::~DFREC()
{
  dfrec_close(m_dfrec);
}
void DFREC::update()
{
  upm_result_t rv = dfrec_update(m_dfrec);

  // we only want to throw on "real" fatal errors
  if (!(rv == UPM_ERROR_OUT_OF_RANGE || rv == UPM_SUCCESS))
    {
      throw std::runtime_error(string(__FUNCTION__)
                               + ": dfrec_update() failed with UPM error "
                               + std::to_string(int(rv)) );
    }
}

void DFREC::setOffset(float offset)
{
  dfrec_set_offset(m_dfrec, offset);
}

void DFREC::setScale(float scale)
{
  dfrec_set_scale(m_dfrec, scale);
}

float DFREC::getEC()
{
  return dfrec_get_ec(m_dfrec);
}

float DFREC::getECValue()
{
  DFREC::update();
  return DFREC::getEC();
}

float DFREC::getVolts()
{
  return dfrec_get_volts(m_dfrec);
}

float DFREC::getNormalized()
{
  return dfrec_get_normalized(m_dfrec);
}

float DFREC::getTemperature()
{
  return dfrec_get_temperature(m_dfrec);
}

void DFREC::setThresholdMinMax(float min, float max)
{
  dfrec_set_threshold_min_max(m_dfrec, min, max);
}

void DFREC::setThreshold1(float thres, float scale, float offset)
{
  dfrec_set_threshold_1(m_dfrec, thres, scale, offset);
}

void DFREC::setThreshold2(float thres, float scale, float offset)
{
  dfrec_set_threshold_2(m_dfrec, thres, scale, offset);
}

void DFREC::setThreshold3(float scale, float offset)
{
  dfrec_set_threshold_3(m_dfrec, scale, offset);
}
