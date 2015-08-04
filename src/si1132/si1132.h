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
#pragma once

#include <string>
#include <mraa/i2c.h>

#include "upm/iLightSensor.h"

#define SI1132_ADDRESS                  0x60

/* REGISTER ADDRESSES */
#define SI1132_REG_PART_ID              0x00
#define SI1132_REG_HW_KEY               0x07
#define SI1132_REG_MEAS_RATE0           0x08
#define SI1132_REG_MEAS_RATE1           0x09
#define SI1132_REG_COMMAND              0x18
#define SI1132_REG_RESPONSE             0x20
#define SI1132_REG_ALS_VIS_DATA0        0x22
#define SI1132_REG_ALS_VIS_DATA1        0x23


 
/* REGISTER VALUES */
#define SI1132_PART_ID                  0x32
#define SI1132_HW_KEY_INIT              0x17 

namespace upm {

/**
 * @brief Si1132 proxmity sensor library
 * @defgroup Si1132 libupm-Si1132
 */

/**
 * @brief C++ API for Si1132 chip (Ambient Light Sensor)
 *
 * The Silicon Labs
 * [Si1132](https://www.silabs.com/Support%20Documents/TechnicalDocs/Si1132.pdf)
 * is a low-power, ultraviolet (UV) index, and ambient light sensor with I2C 
 * digital interface and programmable-event interrupt output.
 *
 * @ingroup Si1132 i2c light
 * @snippet Si1132.cxx Interesting
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
          * Reset sensor to default configuration
          */
        mraa_result_t reset();

        /**
         * Read the lux value from the chip.
         */
        mraa_result_t getValue (float* value);
	
        /**
         * Returns whether the sensor is configured.
         */
        bool isConfigured();

    private:
        mraa_result_t i2cReadRegister(uint8_t reg, uint8_t* value);
        mraa_result_t i2cReadRegister(uint8_t reg, uint16_t* value);
        mraa_result_t clearResponseRegister();
        mraa_i2c_context i2c;
        mraa_result_t status;
};

}
