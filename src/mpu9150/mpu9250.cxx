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
#include <unistd.h>
#include <stdlib.h>

#include "mpu9250.hpp"

using namespace upm;
using namespace std;

MPU9250::MPU9250 (int bus, int address, int magAddress, bool enableAk8975) :
  MPU9150(bus, address, magAddress, enableAk8975)
{
}

MPU9250::~MPU9250()
{
}

float MPU9250::getTemperature()
{
  // this equation is taken from the datasheet.  The 333.87 value was
  // taken from the adafruit code (it is referenced as
  // Temp_Sensitivity in the datasheet, but no value is provided there).
  return (m_temp / 333.87) + 21.0;
}

