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
#include <unistd.h>

#include "ina132.hpp"

using namespace upm;
using namespace std;

INA132::INA132(int pin)
{
    if ( !(m_aio = mraa_aio_init(pin)) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_aio_init() failed, invalid pin?");
      return;
    }
}

INA132::~INA132()
{
  mraa_aio_close(m_aio);
}

float INA132::value()
{
	int val, i;
	float v, vo;
	float sum=0;
	for(i=0;i<10;i++)
	{
		val = mraa_aio_read(m_aio);
                if (val == -1) return -1;
		v = val*5.00/1023;
		sum += v;
		usleep(10000);
	}
	vo = sum/10;
	return vo;
}
