/*
 * Author: Scott Ware <scott.r.ware@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <string>
#include <mraa/i2c.hpp>

#include <interfaces/iLight.hpp>

/* ADDRESS AND NOT_FOUND VALUE */
#define MAX44009_ADDRESS                    ( 0x4A )
#define MAX44009_NOT_FOUND                  ( 0x00 )

/* I2C BUS */
#define MAX44009_I2C_BUS                    ( 1 )

/* REGISTER ADDRESSES */
#define MAX44009_INT_STATUS_ADDR            ( 0x00 )    // R
#define MAX44009_INT_ENABLE_ADDR            ( 0x01 )    // R/W
#define MAX44009_CONFIG_ADDR                ( 0x02 )    // R/W
#define MAX44009_LUX_START_ADDR             ( 0x03 )
#define MAX44009_LUX_LENGTH                 ( 2 )
#define MAX44009_LUX_HIGH                   ( 0 )
#define MAX44009_LUX_LOW                    ( 1 )
#define MAX44009_THR_HIGH_ADDR              ( 0x05 )    // R/W
#define MAX44009_THR_LOW_ADDR               ( 0x06 )    // R/W
#define MAX44009_THR_TIMER_ADDR             ( 0x07 )    // R/W

/* INTERRUPT VALUES */
#define MAX44009_INT_STATUS_OFF             ( 0x00 )
#define MAX44009_INT_STATUS_ON              ( 0x01 )
#define MAX44009_INT_DISABLED               ( 0x00 )
#define MAX44009_INT_ENABLED                ( 0x01 )

/* CONFIGURATION VALUES */
#define MAX44009_CONFIG_DEFAULT             ( 0 << 7 )
#define MAX44009_CONFIG_CONTINUOUS          ( 1 << 7 )
#define MAX44009_CONFIG_AUTO                ( 0 << 6 )
#define MAX44009_CONFIG_MANUAL              ( 1 << 6 )
#define MAX44009_CONFIG_CDR_NORMAL          ( 0 << 3 )
#define MAX44009_CONFIG_CDR_DIVIDED         ( 1 << 3 )
#define MAX44009_CONFIG_INTEGRATION_800ms   ( 0 << 0 )
#define MAX44009_CONFIG_INTEGRATION_400ms   ( 1 << 0 )
#define MAX44009_CONFIG_INTEGRATION_200ms   ( 2 << 0 )
#define MAX44009_CONFIG_INTEGRATION_100ms   ( 3 << 0 )
#define MAX44009_CONFIG_INTEGRATION_50ms    ( 4 << 0 )
#define MAX44009_CONFIG_INTEGRATION_25ms    ( 5 << 0 )
#define MAX44009_CONFIG_INTEGRATION_12ms    ( 6 << 0 )
#define MAX44009_CONFIG_INTEGRATION_6ms     ( 7 << 0 )

/* DEFAULT CONFIGURATION */
#define MAX44009_DEFAULT_CONFIGURATION      ( MAX44009_CONFIG_DEFAULT | \
                                              MAX44009_CONFIG_AUTO | \
                                              MAX44009_CONFIG_CDR_NORMAL | \
                                              MAX44009_CONFIG_INTEGRATION_100ms )

/* MISCELLANEOUS */
#define MAX44009_OVERRANGE_CONDITION        ( 0x0F )

namespace upm {

/**
 * @brief MAX44009 Ambient Light Sensor
 * @defgroup max44009 libupm-max44009
 * @ingroup maxim i2c light ilightsensor
 */

/**
 * @brief C++ API for MAX44009 chip (Ambient Light Sensor)
 *
 * The Maxim Integrated
 * [MAX44009](http://datasheets.maximintegrated.com/en/ds/MAX44009.pdf)
 * is Industry's Lowest-Power Ambient Light Sensor with ADC.
 *
 * @library max44009
 * @sensor max44009
 * @comname I2C Low-power Digital Ambient Light Sensor
 * @altname MAX44009
 * @type light
 * @man maxim
 * @con i2c
 * @web https://www.maximintegrated.com/en/products/analog/sensors-and-sensor-interface/MAX44009.html
 * @if ilightsensor
 *
 * @snippet max44009.cxx Interesting
 *
 */
class MAX44009 : virtual public iLight {
    public:
        /**
         * Instanciates a MAX44009 object
         *
         * @param bus number of used bus
         * @param devAddr address of used i2c device
         */
        MAX44009 (int bus = MAX44009_I2C_BUS, int devAddr = MAX44009_ADDRESS);

        /**
         * MAX44009 object destructor, basicaly it close i2c connection.
         */
        ~MAX44009 ();

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

        virtual const char* getModuleName() { return "max44009"; }

    private:
        /* Disable implicit copy and assignment operators */
        MAX44009(const MAX44009&) = delete;
        MAX44009 &operator=(const MAX44009&) = delete;

        mraa::Result reset();

        int m_maxControlAddr;
        mraa::I2c* i2c;
        mraa::Result status;
};

}
