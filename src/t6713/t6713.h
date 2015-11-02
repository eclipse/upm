/*
 * Author: Scott Ware <scott.r.ware@intel.com>
 * Copyright (c) 2014 Intel Corporation.
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


#include "mraa/i2c.hpp"
#include "upm/iCO2Sensor.h"

namespace upm {

/**
 * @brief Amphenol Telaire 6713 Series CO2 Module
 * @defgroup t6713 libupm-t6713
 * @ingroup i2c telaire gas ico2sensor
 */

/**
 * @brief C++ API for Amphenol Telaire 6713 Series CO2 Module
 *
 * The Telaire 6713 Series CO2 Module
 * [6713](http://amphenol-sensors.com/en/products/co2/co2-modules/3215-t6700#specifications-t6713)
 * is a CO2 Module sensor.
 *
 * @library t6713
 * @sensor t6713
 * @comname Telaire 6713 Series CO2 Module
 * @altname T6713
 * @type gaseous
 * @man amphenol
 * @con i2c
 * @if ic02sensor

 */
class T6713 : public ICO2Sensor {
    public:
        /**
         * Instantiates a T6713 object
         *
         * @param bus number of used bus
         */
        T6713 (int bus);

        /**
         * SI7005 object destructor.
         */
        ~T6713 ();

        /**
         * Returns whether the sensor is configured.
         */
        bool isConfigured();

        /**
         * Returns sensor module name
         */
        const char* getModuleName();          

        /**
         * Get relative humidity measurement.
         */
        uint16_t getPpm ();

    private:
        int bus;
        mraa::I2c* i2c;
        mraa::Result status;
};

}