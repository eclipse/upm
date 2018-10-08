/*
 * Author: Zion Orent <zorent@ics.com>
 * Copyright (c) 2015 Intel Corporation.
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

#include "groveo2.hpp"

using namespace upm;
using namespace std;

GroveO2::GroveO2(int pin)
{
    if ( !(m_aio = mraa_aio_init(pin)) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_aio_init() failed, invalid pin?");
      return;
    }
}

GroveO2::~GroveO2()
{
  mraa_aio_close(m_aio);
}

float GroveO2::voltageValue()
{
	int val = mraa_aio_read(m_aio);
	if (val == -1) return -1.0f;
	float sensorVoltage = (val/1024.0) * 5.0;
	sensorVoltage = (sensorVoltage/201.0) * 10000.0;
	return sensorVoltage;
}

float GroveO2::getConcentration()
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

