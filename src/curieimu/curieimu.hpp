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

#define FIRMATA_START_SYSEX                 0xF0
#define FIRMATA_END_SYSEX                   0xF7
#define FIRMATA_CURIE_IMU                   0x11
#define FIRMATA_CURIE_IMU_READ_ACCEL        0x00
#define FIRMATA_CURIE_IMU_READ_GYRO         0x01
#define FIRMATA_CURIE_IMU_READ_TEMP         0x02
#define FIRMATA_CURIE_IMU_SHOCK_DETECT      0x03
#define FIRMATA_CURIE_IMU_STEP_COUNTER      0x04
#define FIRMATA_CURIE_IMU_TAP_DETECT        0x05
#define FIRMATA_CURIE_IMU_READ_MOTION       0x06

class CurieImu {
    public:
        /**
         * Instantiates a CurieImu object
         *
         * @param subplatform_offset Subplatform offset, default 512
         */
        CurieImu (int subplatform_offset=512);

        /**
         * Destructor for a CurieImu object
         */
        ~CurieImu();

        /**
         * Returns the Temperature
         */
        int16_t getTemperature();

        pthread_mutex_t m_responseLock;
        pthread_cond_t m_responseCond;
        char* m_results;

    private:
        mraa_firmata_context m_firmata;
};

}
