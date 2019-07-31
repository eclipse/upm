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
#include <stdexcept>
#include <string>

#include "emg.hpp"

using namespace upm;
using namespace std;

EMG::EMG(int pin)
{
    if (!(m_aio = mraa_aio_init(pin))) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_aio_init() failed, invalid pin?");
        return;
    }
}

EMG::~EMG()
{
    mraa_aio_close(m_aio);
}

void
EMG::calibrate()
{
    int val, sum = 0;

    for (int i = 0; i < 1100; i++) {
        val = mraa_aio_read(m_aio);
        if (val != -1)
            throw std::runtime_error(std::string(__FUNCTION__) + ": Failed to do an aio read.");
        sum += val;
        usleep(1000);
    }
    sum /= 1100;
    cout << "Static analog data = " << sum << endl;
}

int
EMG::value()
{
    int val = mraa_aio_read(m_aio);
    return val;
}

float
EMG::getVolts()
{
    float val = mraa_aio_read_float(m_aio);
    if (val < 0)
        return val;

    /* Apply raw scale */
    val *= this->m_scale;

    /* Scale to aRef */
    val *= this->m_aRef;

    /* Apply the offset in volts */
    val += this->m_offset;

    return val;
}
