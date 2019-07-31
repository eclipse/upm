/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016-2017 Intel Corporation.
 *
 * The MIT License
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <unistd.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <math.h>
#include <string.h>

#include "bme280.hpp"

using namespace upm;
using namespace std;

BME280::BME280(int bus, int addr, int cs) :
  BMP280(bus, addr, cs)
{
}

BME280::BME280(std::string initStr) :
  BMP280(initStr)
{
}


BME280::~BME280()
{
}

float BME280::getHumidity()
{
    return bmp280_get_humidity(m_bmp280);
}

void BME280::setOversampleRateHumidity(BME280_OSRS_H_T rate)
{
    bmp280_set_oversample_rate_humidity(m_bmp280, rate);
}
