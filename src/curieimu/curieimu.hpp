/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
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
#pragma once

#include <mraa/firmata.h>

namespace upm {

/**
 * @brief CurieIMU sensor for Genuino 101 running Firmata
 * @defgroup curieimu libupm-curieimu
 * @ingroup firmata
 */

/**
 * @library curieimu
 * @sensor curieimu
 * @comname Curie IMU sensor over Genuino 101 running Firmata
 * @altname Curie Firmata IMU
 * @type firmata
 * @man firmata imu genuino
 * @con firmata
 *
 * @brief API for the Curie IMU via Firmata
 *
 * Curie IMU is a 6-axxis acclerometer
 *
 * This module has been tested on an Genuino 101 running ConfigurableFirmata with CurieIMU
 *
 * @snippet curieimu.cxx Interesting
 */

class CurieImu {
    public:
        /**
         * Instantiates a CurieImu object
         *
         * @param subplatform_offset Subplatform offset, default 512
         */
        CurieImu (int subplatform_offset=512);

        /**
         * Returns the Temperature
         */
        int16_t getTemperature();

    private:
        mraa_firmata_context m_firmata;
};

}
