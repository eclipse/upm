/*
 * Author: Henry Bruce <henry.bruce@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <string>
#include "mraa/i2c.hpp"
#include <interfaces/iLight.hpp>


namespace upm {

/**
 * @brief SI1132 UV and Visible Light Sensor
 * @defgroup Si1132 libupm-Si1132
 * @ingroup silabs i2c light ilightsensor
 */

/**
 * @library si1132
 * @sensor si1132
 * @comname UV and Ambient Light Sensor
 * @altname Si1132
 * @type light
 * @man silabs
 * @con i2c
 * @web http://www.silabs.com/products/sensors/infraredsensors/Pages/si1132.aspx
 * @web https://www.silabs.com/Support%20Documents/TechnicalDocs/Si1132.pdf
 *
 * @brief API for Si1132 UV and Visible Light Sensor
 *
 * The Silicon Labs
 * [Si1132](https://www.silabs.com/Support%20Documents/TechnicalDocs/Si1132.pdf)
 * is a low-power, ultraviolet (UV) index, and ambient light sensor with I2C
 * digital interface and programmable-event interrupt output.
 *
 * @snippet si1132.cxx Interesting
 */
class SI1132 : virtual public iLight {
    public:
        /**
         * Instanciates a Si1132 object
         *
         * @param bus number of used bus
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

        /**
         * Gets the luminance value from the sensor
         *
         * @return The measured light intensity value in Lux
         */
        virtual float getLuminance();

        virtual const char* getModuleName() { return "si1132"; }

    private:
        /* Disable implicit copy and assignment operators */
        SI1132(const SI1132&) = delete;
        SI1132 &operator=(const SI1132&) = delete;

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
