/*
 * Author: Lay, Kuan Loon <kuan.loon.lay@intel.com>
 * Copyright (c) 2016 Intel Corporation.
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

#include "apds9930.hpp"

using namespace upm;

APDS9930::APDS9930(int device)
{
    if (!(m_iio = mraa_iio_init(device))) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_iio_init() failed, invalid device?");
        return;
    }
}

APDS9930::~APDS9930()
{
    if (m_iio)
        mraa_iio_close(m_iio);
}

int
APDS9930::getAmbient()
{
    int iio_value = 0;
    mraa_iio_read_int(m_iio, "in_illuminance_input", &iio_value);
    return iio_value;
}

int
APDS9930::getProximity()
{
    int iio_value = 0;
    mraa_iio_read_int(m_iio, "in_proximity_raw", &iio_value);
    return iio_value;
}

bool
APDS9930::enableProximity(bool enable)
{
    if (enable)
        mraa_iio_write_int(m_iio, "in_proximity_en", 1);
    else
        mraa_iio_write_int(m_iio, "in_proximity_en", 0);

    return true;
}

bool
APDS9930::enableIlluminance(bool enable)
{
    if (enable)
        mraa_iio_write_int(m_iio, "in_illuminance_en", 1);
    else
        mraa_iio_write_int(m_iio, "in_illuminance_en", 0);

    return true;
}