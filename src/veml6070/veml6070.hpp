/*
 * Author: Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
 
#pragma once

#include "veml6070.h"

namespace upm {
/**
 * @brief VEML6070 UV Sensor
 * @defgroup veml6070 libupm-veml6070
 * @ingroup vishay i2c light
 */

/**
 * @library veml6070
 * @sensor veml6070
 * @comname Vishay VEML6070 UV Sensor
 * @type other
 * @man Vishay
 * @web http://www.vishay.com/docs/84277/veml6070.pdf
 * @con i2c
 * @kit other
 *
 * @brief API for the Vishay VEML6070 UV Sensor
 *
 * VEML6070 is an advanced ultraviolet (UV) light sensor with
 * I2C protocol interface and designed by the CMOS process.
 * It is easily operated via a simple I2C command. The active
 * acknowledge (ACK) feature with threshold windows setting
 * allows the UV sensor to send out a UVI alert message.
 * Under a strong solar UVI condition, the smart ACK signal
 * can be easily implemented by the software programming.
 * VEML6070 incorporates a photodiode, amplifiers, and
 * analog / digital circuits into a single chip. VEML6070's
 * adoption of FiltronTM UV technology provides the best
 * spectral sensitivity to cover UV spectrum sensing. It has an
 * excellent temperature compensation and a robust refresh
 * rate setting that does not use an external RC low pass filter.
 * VEML6070 has linear sensitivity to solar UV light and is
 * easily adjusted by an external resistor. Software shutdown
 * mode is provided, which reduces power consumption to be
 * less than 1 uA. VEML6070's operating voltage ranges from
 * 2.7 V to 5.5 V.
 *
 * @image html veml6070.jpg
 * @snippet veml6070.cxx Interesting
 */
    class VEML6070 {

    public:
        /**
         * VEML6070 constructor
         *
         * @param bus i2c bus to be used
         * @param devAddress i2c address of the sensor
         */
        VEML6070(int bus);
        /**
         * VEML6070 destructor
         */
        ~VEML6070();

        /**
         * Function to get the UV value.
         *
         * @return int16_t UV value
         */
        int getUVIntensity();

        /**
         * Function to allow the user to set the intergration time
         * for the sensor.
         * Integration time:
         * Bit Setting   |  Description  |           RSet
         *  IT1   IT0    |               | 300 k-ohms | 600 k-ohms
         *   0     0     |      1/2T     |   62.5 ms  |   125 ms
         *   0     1     |       1T      |   125 ms   |   250 ms
         *   1     0     |       2T      |   250 ms   |   500 ms
         *   1     1     |       4T      |   500 ms   |  1000 ms
         *
         * @param time veml6070_integration_time_t
         */
        void setIntegrationTime(veml6070_integration_time_t time);

    private:
        veml6070_context m_veml6070;
        VEML6070(const VEML6070& src) { /* do not create copied constructor */}
        VEML6070& operator=(const VEML6070&) {return *this;}
    };
}
