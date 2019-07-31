/*
 * Author: Norbert Wesp <nwesp@phytec.de>
 * Copyright (c) 2017 Phytec Messtechnik GmbH.
 *
 * based on: RIOT-driver hdc1000 by Johann Fischer <j.fischer@phytec.de>
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
#include <math.h>
#include <interfaces/iHumidity.hpp>
#include <interfaces/iTemperature.hpp>

#define HDC1000_NAME "hdc1000"
#define HDC1000_i2C_ADDRESS         0x43
#define HDC1000_MANUFACTURER_ID     0x5449  /* ID of Texas Instruments */
#define HDC1000_DEVICE_ID           0x1000  /* ID of HDC1000 device */

/* HDC1000 Register Map */
#define HDC1000_TEMPERATURE         0x00
#define HDC1000_HUMIDITY            0x01
#define HDC1000_CONFIGURATION       0x02
#define HDC1000_MANUFACTURER_ID_REG 0xFE
#define HDC1000_DEVICE_ID_REG       0xFF

/* HDC1000 configuration register bitmap */
#define HDC1000_RST_SOFT            0x8000
#define HDC1000_HEAT                (1 << 13)
#define HDC1000_SEQ_MODE            (1 << 12)
#define HDC1000_BTST_LOW            (1 << 11)
#define HDC1000_TRES_14             (0)
#define HDC1000_TRES_11             (1 << 10)
#define HDC1000_HRES_14             (0)
#define HDC1000_HRES_11             (1 << 8)
#define HDC1000_HRES_8              (1 << 9)

#define SLEEP_SEC                   (1000*1000)

namespace upm {

/**
 * @brief HDC1000 Temperature and Humidity Sensor
 * @defgroup hdc1000 libupm-hdc1000
 * @ingroup ti i2c temp
 */

/**
 * @library hdc1000
 * @sensor hdc1000
 * @comname HDC1000 Temperature & Humidity Sensor
 * @type temp
 * @man ti
 * @web http://www.ti.com/lit/ds/symlink/hdc1000.pdf
 * @con i2c
 *
 * @brief API for the HDC1000 Temperature & Humidity Sensor
 *
 * Description in web-link above:
 * The HDC1000 is a digital humidity sensor with integrated temperature sensor
 * that provides excellent measurement accuracy at very low power. The device
 * measures humidity based on a novel capacitive sensor. The humidity and
 * temperature sensors are factory calibrated. The innovative
 * WLCSP (Wafer Level Chip Scale Package) simplifies board design with the use
 * of an ultra-compact package. The sensing element of the HDC1000 is placed on
 * the bottom part of the device, which makes the HDC1000 more robust against
 * dirt, dust, and other environmental contaminants. The HDC1000 is functional
 * within the full -40 to +125 [degC] temperature range.
 *
 * @snippet hdc1000.cxx Interesting
 */
class HDC1000 : virtual public iHumidity, virtual public iTemperature {
    public:
        /**
         * Instantiates an HDC1000 object
         *
         * @param bus Number of the used bus
         * @param devAddr Address of the used I2C device
         */
        HDC1000 (int bus, int devAddr=HDC1000_i2C_ADDRESS);

        /**
         * Check device_id of sensor
         *
         * @return  0 on succes
         *         -1 on error
         */
        int checkID(void);

        /**
         * Performs a soft reset of the HDC1000 device to ensure
         * it is in a known state.
         */
        void resetSensor(void);

        /**
         * Initiates a temperature/pressure mesasurement and waits
         * to complete.
         */
        void sampleData(void);

        /**
         * Get the current measured humidity [RH]
         *
         * @param bSampleData Flag to read sensor
         * @return The humidity sensor temp in degC
         */
        float getHumidity(int bSampleData);

        /**
         * Get the current measured humidity [RH]
         *
         * @return The humidity sensor temp in degC
         */
        virtual float getHumidity();

        /**
         * Get the humidity cell temperature [degC]
         *
         * @param bSampleData Flag to read sensor
         * @return The humidity sensor temp in degC
         */
        float getTemperature(int bSampleData);

        /**
         * Get the humidity cell temperature [degC]
         *
         * @return The humidity sensor temp in degC
         */
        virtual float getTemperature();

    private:

        std::string m_name;

        int m_controlAddr;
        int m_bus;
        mraa::I2c m_i2ControlCtx;

        int32_t m_temperature;
        int32_t m_humidity;
};

}
