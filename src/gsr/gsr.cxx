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

#include "gsr.hpp"

using namespace upm;
using namespace std;

GSR::GSR(int pin)
{
    if ( !(m_aio = mraa_aio_init(pin)) )
        throw std::invalid_argument(std::string(__FUNCTION__) +
                ": mraa_aio_init() failed, invalid pin?");
}

GSR::~GSR()
{
    mraa_aio_close(m_aio);
}

void GSR::calibrate()
{
    int val, threshold, sum = 0;

    for(int i=0; i<500; i++)
    {
        val = mraa_aio_read(m_aio);
        if (val < 0)
            throw std::runtime_error(std::string(__FUNCTION__) +
                ": Failed to do an aio read.");
        sum += val;
        usleep(5000);
    }
    threshold = sum / 500;
    cout << "Threshold = " << threshold << endl;
}

int GSR::value()
{
    int val = mraa_aio_read(m_aio);
    return val;
}

float GSR::getECValue()
{
	return (float) GSR::value();
}
