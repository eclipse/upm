/*
 * Author: Norbert Wesp <nwesp@phytec.de>
 * Copyright (c) 2017 Phytec Messtechnik GmbH.
 *
 * based on: RIOT-driver tmp006 by Johann Fischer <j.fischer@phytec.de>
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

#define TMP006_NAME "TMP006"
#define TMP006_I2C_ADDRESS     0x41
#define TMP006_MANUFACTURER_ID 0x5449
#define TMP006_DEVICE_ID       0x0067

#define TMP007_I2C_ADDRESS     0x40
// this should actually be split
#define TMP007_DEVICE_ID       0x0078

#ifndef TMP006_CONVERSION_TIME
#define TMP006_CONVERSION_TIME 1E6 /**< Default Conversion Time in us */
#endif

/* TMP006 Register Map */
#define TMP006_SENSOR_VOLTAGE      0x00   /* Sensor voltage register */
#define TMP006_LOCAL_TEMPERATURE   0x01   /* Ambient temperature register */
#define TMP006_CONFIGURATION       0x02
#define TMP006_MANUFACTURER_ID_REG 0xFE
#define TMP006_DEVICE_ID_REG       0xFF

/* TMP007 Register Map */
#define TMP007_DEVICE_ID_REG       0x1F

/* TMP006 configuration register bitmap */
#define TMP006_RST_SOFT         (1 << 15)

#define TMP006_CONFIG_MOD_SHIFT 12
#define TMP006_CONFIG_MOD_MASK  0x7000
#define TMP006_CONFIG_MOD(x)    (((uint16_t)(((uint16_t)(x))<<\
                                TMP006_CONFIG_MOD_SHIFT))\
                                &TMP006_CONFIG_MOD_MASK)
#define TMP006_CONFIG_MOD_CC    0x07
#define TMP006_CONFIG_MOD_OFF   0x00

#define TMP006_CONFIG_CR_SHIFT  9
#define TMP006_CONFIG_CR_MASK   0x0E00
#define TMP006_CONFIG_CR(x)     (((uint16_t)(((uint16_t)(x))<<\
                                TMP006_CONFIG_CR_SHIFT))\
                                &TMP006_CONFIG_CR_MASK)
#define TMP006_CONFIG_CR_AS1  0x00 /* Conversion Time 0.25 s, AVG Samples: 1 */
#define TMP006_CONFIG_CR_AS2  0x01 /* Conversion Time 0.5 s, AVG Samples: 2 */
#define TMP006_CONFIG_CR_AS4  0x02 /* Conversion Time 1 s, AVG Samples: 4 */
#define TMP006_CONFIG_CR_AS8  0x03 /* Conversion Time 2 s, AVG Samples: 8 */
#define TMP006_CONFIG_CR_AS16 0x04 /* Conversion Time 4 s, AVG Samples: 16 */
#define TMP006_CONFIG_CR_DEF  TMP006_CONFIG_CR_AS4 /**< Default for Testing */

#define TMP006_DRDY_EN          (1 << 8)
#define TMP006_DRDY_DATA_RDY    (1 << 7)

/* constant values for data conversion */
#ifndef TMP006_CCONST_S0
#define TMP006_CCONST_S0       6.4E-14    /* Calibration Factor */
#endif

#define TMP006_CCONST_A1       1.75E-3   /* Constant \f$a_{\mathrm{1}}\f$ */
#define TMP006_CCONST_A2       -1.678E-5 /* Constant \f$a_{\mathrm{2}}\f$ */
#define TMP006_CCONST_TREF     298.15    /* Constant \f$T_{\mathrm{REF}}\f$ */
#define TMP006_CCONST_B0       -2.94E-5  /* Constant \f$b_{\mathrm{0}}\f$ */
#define TMP006_CCONST_B1       -5.7E-7   /* Constant \f$b_{\mathrm{1}}\f$ */
#define TMP006_CCONST_B2       4.63E-9   /* Constant \f$b_{\mathrm{2}}\f$ */
#define TMP006_CCONST_C2       13.4      /* Constant \f$c_{\mathrm{2}}\f$ */
#define TMP006_CCONST_LSB_SIZE 156.25E-9 /* Sensor Voltage Register LSB Size */

typedef enum {
    TMP006_SEN,
    TMP007_SEN } tmp_t;

namespace upm {

/**
 * @brief TMP006 Infrared-Thermopile Sensor
 * @defgroup tmp006 libupm-tmp006
 * @ingroup ti i2c temp
 */

/**
 * @library tmp006
 * @sensor tmp006
 * @comname TMP006 IR-Thermopile Sensor
 * @type temp
 * @man ti
 * @web http://www.ti.com/product/TMP006/datasheet/abstract#SBOS5183423
 * @con i2c
 *
 * @brief API for the TMP006 IR-Thermopile Sensor
 *
 * Description in web-link above:
 * The TMP006 and TMP006B are fully integrated MEMs thermopile sensors that
 * measure the temperature of an object without having to be in direct contact.
 * The thermopile absorbs passive infrared energy from an object at wavelengths
 * between 4 um to 16 um within the end-user defined field of view.
 * The corresponding change in voltage across the thermopile is digitized and
 * reported with the on-chip die thermal sensor measurement through an I2C- and
 * SMBus-compatible interface. With this data, the target object temperature
 * can be calculated by an external processor.
 * The Infrared thermopile sensor is specified to operate from 
 * -40 to +125 [degC]. It is possible to measure an object temperature beyond
 * the device operating range as long as the device itself does not exceed the
 * operating temperature range (-40 to +125 [degC]).
 *
 * @snippet tmp006.cxx Interesting
 */
class TMP006 {
    public:
        /**
         *
         * Instantiates an TMP006 object
         *
         * @param bus Number of the used bus
         * @param conv_rate Conversation rate (allowed between 0 and 4)
         * @param devAddr Address of the used I2C device
         */
        TMP006 (int bus, uint8_t conv_rate=TMP006_CONFIG_CR_DEF,
                                  int devAddr=TMP006_I2C_ADDRESS);

        /**
         * Check device_id of sensor
         *
         * @return  0 on succes
         *         -1 on error
         */
        int checkID(void);

        /**
         * Performs a soft reset of the TMP006 device to ensure
         * it is in a known state.
         */
        void resetSensor(void);

        /**
         * Set active mode, this enables periodic measurements.
         */
        void setActive(void);

        /**
         * Set standby mode, this disables periodic measurements.
         */
        void setStandby(void);

        /**
         * Read out data of regs and call further function
         * Also saves converted value to variable
         */
        int sampleData(void);

        /**
         * Convert raw sensor values to temperature.
         *
         * @param rawv Object voltage value
         * @param rawt Raw die temperature value
         * @param tamb Converted ambient temperature
         * @param tobj Converted object temperature
         */
        void convert_data(int16_t rawv,int16_t rawt, float *tamb, float *tobj);

        /**
         * Get the calculated object temperature [degC]
         *
         * @param bSampleData Flag to read sensor
         * @return The object temp in degC on success
         *         and -273.2 on 'conversation in progress'
         */
        float getTemperature(int bSampleData = 0);

        /**
         * Get value of configuration reg
         */
        uint16_t getConfig(void);

    private:

        std::string m_name;

        int m_controlAddr;
        int m_bus;
        mraa::I2c m_i2ControlCtx;
        tmp_t sensorType;

        int32_t m_temperature;
};

}
