/*
 * Author: Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2017 Intel Corporation.
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

#include "abpdrrt005pg2a5.h"

namespace upm {
/**
 * @defgroup abpdrrt005pg2a5 libupm-abpdrrt005pg2a5
 * @ingroup honeywell i2c
 */

/**
 * @library abpdrrt005pg2a5
 * @sensor abpdrrt005pg2a5
 * @comname ABP Honeywell Pressure Sensor
 * @type other
 * @man Honeywell
 * @web https://sensing.honeywell.com/honeywell-sensing-basic-board-mount-pressure-sensors-abp-series-datasheet-323005128-c-en.pdf
 * @con i2c
 * @kit other
 *
 * @brief API for the ABP Honeywell Pressure Sensor
 *
 * This is the UPM Module for the ABPDRRT005PG2A5 Honeywell Pressure
 * sensor. This sensor uses an onboard ASIC to output values
 * of pressure that are updated at approximately 2 KHz. It is capable
 * of detecting pressure in the 0-5 psi range and it has an i2c
 * based interface. Temperature calculation using this sensor
 * is not recommended as the values are not calibrated.
 *
 * @image html abpdrrt005pg2a5.jpg
 * @snippet abpdrrt005pg2a5.cxx Interesting
 */

    class ABPDRRT005PG2A5 {

    public:
        /**
         * ABPDRRT005PG2A5 constructor
         *
         * @param bus i2c bus to be used
         * @param devAddress i2c address of the sensor
         */
        ABPDRRT005PG2A5(int bus, int devAddress);
        /**
         * ABPDRRT005PG2A5 destructor
         */
        ~ABPDRRT005PG2A5();
        /**
         * Get pressure in pounds per square inch
         *
         * @return pressure in psi
         */
        float get_pressure_psi();
        /**
         * Get pressure in pascals
         *
         * @return pressure in pascal
         */
        float get_pressure_pascal();

    private:
        abpdrrt005pg2a5_context m_abpdrrt005pg2a5;
    };
}