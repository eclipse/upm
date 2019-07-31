/*
 * Author: Zion Orent <zorent@ics.com>
 * Copyright (c) 2015 Intel Corporation.
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

#include "o2.hpp"

using namespace upm;
using namespace std;

O2::O2(int pin)
{
    if ( !(m_aio = mraa_aio_init(pin)) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_aio_init() failed, invalid pin?");
      return;
    }
}

O2::~O2()
{
  mraa_aio_close(m_aio);
}

float O2::voltageValue()
{
	int val = mraa_aio_read(m_aio);
	if (val == -1) return -1.0f;
	float sensorVoltage = (val/1024.0) * 5.0;
	sensorVoltage = (sensorVoltage/201.0) * 10000.0;
	return sensorVoltage;
}

float O2::getConcentration()
{
  float value;
  /* Read normalized value */
    value = mraa_aio_read_float(m_aio);
    if (value < 0.0)
        return -1;

    /* Convert to %oxygen
       Datasheet for grove o2 shows a linear response for the sensor.  Assuming
       20.5% oxygen @ 25 celsius, with an gain = 1 + 12k/100 = 121, a
       dynamic range of 0->25% oxygen, and opamp rails of 0->3.3v (the grove o2
       sensor uses a high-accuracy 3.3v regulator),
     */
    value *= 25 * 5 / 3.3;

  return value;
}
