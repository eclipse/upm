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

#include "abp.h"

namespace upm {
/**
 * @brief Honeywell ABP Pressure Sensors
 * @defgroup abp libupm-abp
 * @ingroup honeywell i2c
 */

/**
 * @library abp
 * @sensor abp
 * @comname Honeywell ABP Pressure and Temperature Sensor
 * @type other
 * @man Honeywell
 * @web https://sensing.honeywell.com/honeywell-sensing-basic-board-mount-pressure-sensors-abp-series-datasheet-323005128-c-en.pdf
 * @con i2c
 * @kit other
 *
 * @brief API for the ABP Honeywell Pressure Sensor
 *
 * This is the UPM Module for the ABP Honeywell Pressure and Temp
 * sensor. This sensor uses an onboard ASIC to output values
 * of pressure that are updated at approximately 2 KHz. It is capable
 * of detecting pressure in the 0-5 psi range and it has an i2c
 * based interface. Temperature calculation using this driver is possible
 * but not all ABP sensors support that functionality.
 *
 * NOTE: This driver supports only the I2C based ABP sensors, it does
 * not currently support SPI and Analog based sensors.
 *
 * @image html abp.jpg
 * @snippet abp.cxx Interesting
 */

    class ABP {

    public:
        /**
         * ABP constructor
         *
         * @param bus i2c bus to be used
         * @param devAddress i2c address of the sensor
         */
        ABP(int bus, int devAddress);
        /**
         * ABP destructor
         */
        ~ABP();
        /**
         * Function to get the compensated pressure.
         * The datasheet provides a way to calculate the pressure.
         * In order to know the units take a look at the datasheet.
         *
         * @return float compensated temperature value
         */
        float getPressure();

        /**
         * Function to retrieve the compensated temperature values.
         * All ABP seem to calculate temperature in degree Celsius.
         * (The above fact needs to be verified)
         * NOTE: This functionality is not available in all the ABP sensors.
         *
         * @return float compensated temperature value
         */
        float getTemperature();

         /**
          * This functio has to be called before calling either of the get
          * temperature or get pressure functions. This function updates the
          * device context with the latest values. Not calling this function
          * will lead to stale values.
          */
        void update();

        /**
         * Function to set the max pressure that the sensor
         * can read, can be retrieved from the datasheet, if this function is
         * not explicitly called to the set the min value, then a default max
         * value of 5 will be set.
         *
         * @param max int maximum pressure value possible
         */
        void setMaxPressure(int max);

        /**
         * Function to set the min pressure that the sensor
         * can read, can be retrieved from the datasheet, if this function is
         * not explicitly called to the set the min value, then a default min
         * value of 0 will be set.
         *
         * @param min int minimum pressure value possible
         */
        void setMinPressure(int min);
    private:
        abp_context m_abp;
        ABP(const ABP& src) { /* do not create copied constructor */}
        ABP& operator=(const ABP&) {return *this;}
    };
}