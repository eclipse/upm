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
#include <stdexcept>
#include <string>

#include "maxsonarez.hpp"

using namespace std;
using namespace upm;

MAXSONAREZ::MAXSONAREZ(int pin, float aref)
{
    if (!(m_aio = mraa_aio_init(pin))) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_aio_init() failed, invalid pin?");
        return;
    }

    m_aRes = (1 << mraa_aio_get_bit(m_aio));
    m_aref = aref;

    // volt's per inch of this sensor
    m_vI = (m_aref / MAXSONAREZ_RES);
}

MAXSONAREZ::~MAXSONAREZ()
{
    mraa_aio_close(m_aio);
}

int
MAXSONAREZ::inches()
{
    int val = mraa_aio_read(m_aio);
    if (val == -1) {
        return -1;
    }
    float volts = float(val) * (m_aref / m_aRes);

    return int(volts / m_vI);
}

float
MAXSONAREZ::getDistance()
{
    return inches() * 2.54;
}
