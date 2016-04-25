/*
 * Author: Henry Bruce <henry.bruce@intel.com>
 * Copyright (c) 2015 Intel Corporation.
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

#include <string>
#include "mraa/i2c.hpp"
#include "upm/iLightSensor.hpp"


namespace upm {

/**
 * @brief Si1132 UV and visible light sensor library
 * @defgroup Si1132 libupm-Si1132
 * @ingroup silabs i2c light ilightsensor
 */

/**
 * @brief API for Si1132 UV and Visible Light Sensor
 *
 * The Silicon Labs
 * [Si1132](https://www.silabs.com/Support%20Documents/TechnicalDocs/Si1132.pdf)
 * is a low-power, ultraviolet (UV) index, and ambient light sensor with I2C
 * digital interface and programmable-event interrupt output.
 *
 * @library si1132
 * @sensor si1132
 * @comname Si1132 Light Sensor
 * @altname Si1132
 * @type light
 * @man silabs
 * @con i2c
 * @if ilightsensor

 */
class SI1132 : public ILightSensor {
    public:
        /**
         * Instanciates a Si1132 object
         *
         * @param bus number of used bus
         * @param devAddr address of used i2c device
         */
        SI1132 (int bus);

        /**
         * Si1132 object destructor, basicaly it close i2c connection.
         */
        ~SI1132 ();

        /**
         * Read the raw visible light value
         */
        uint16_t getVisibleRaw();

        /**
         * Read the lux value
         */
        double getVisibleLux();

        const char* getModuleName() { return "si1132"; }

    private:
        mraa::Result reset();
        mraa::Result clearResponseRegister();
        mraa::Result runCommand(uint8_t command);
        mraa::Result readParam(uint8_t param, uint8_t* value);
        mraa::Result writeParam(uint8_t param, uint8_t value);
        void sleepMs(int mseconds);

        mraa::I2c* i2c;
        mraa::Result status;
};

}
