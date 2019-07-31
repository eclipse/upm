/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015-2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <stdexcept>
#include "lm35.hpp"

using namespace std;
using namespace upm;

LM35::LM35(int pin, float aref) :
    m_lm35(lm35_init(pin, aref))
{
    if (!m_lm35)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lm35_init() failed");
}

LM35::~LM35()
{
    lm35_close(m_lm35);
}

float LM35::getTemperature()
{
    float temp;
    upm_result_t rv = lm35_get_temperature(m_lm35, &temp);

    if (rv)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": lm35_get_temperature() failed");

  return temp;
}

void LM35::setScale(float scale)
{
    lm35_set_scale(m_lm35, scale);
}

void LM35::setOffset(float offset)
{
    lm35_set_offset(m_lm35, offset);
}
