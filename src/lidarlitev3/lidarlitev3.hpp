/*
 * Author: Saloni Jain <saloni.jain@tcs.com>
 * Author: Niti Rohilla <niti.rohilla@tcs.com>
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
#include <interfaces/iDistance.hpp>

#define ADDR                  0x62 // device address

// registers address
#define ACQ_COMMAND           0x00 // Device Command
#define STATUS                0x01 // System Status
#define SIG_COUNT_VAL         0x02 // Maximum Acquisition Count
#define ACQ_CONFIG_REG        0x04 // Acquisition Mode Control
#define VELOCITY              0x09 // Velocity measurement output
#define PEAK_CORR             0x0C // Peak value in correlation record
#define NOISE_PEAK            0x0D // Correaltion record noise floor
#define SIGNAL_STRENGTH       0x0E // Received signal strength
#define FULL_DELAY_HIGH       0x0F // Distance measurement high byte
#define FULL_DELAY_LOW        0x10 // Distance measurement low byte
#define OUTER_LOOP_COUNT      0x11 // Burst measurement count conttrol
#define REF_COUNT_VAL         0x12 // Reference acquisition count
#define LAST_DELAY_HIGH       0x14 // Previous distance measurement high byte
#define LAST_DELAY_LOW        0x15 // Previous distance measurement low byte
#define UNIT_ID_HIGH          0x16 // Serial number high byte
#define UNIT_ID_LOW           0x17 // Serial number low byte
#define I2C_ID_HIGH           0x18 // Write serial number high byte for I2C address unclock
#define I2C_ID_LOW            0x19 // Write serial number low byte for I2X address unlock
#define I2C_SEC_ADDR          0x1A // Write new I2C address after unlock
#define THRESHOLD_BYPASS      0x1C // Peak detection threshold bypass
#define I2C_CONFIG            0x1E // Default address response control
#define COMMAND               0x40 // State command
#define MEASURE_DELAY         0x45 // Delay between automatic measurements
#define PEAK_BCK              0x4C // Second largest peak value on correlation record
#define CORR_DATA             0x52 // Correlation record data lo byte
#define CORR_DATA_SIGN        0x53 // Correlation record data high byte
#define ACQ_SETTINGS          0x5D // Correaltion record memory bank select
#define POWER_CONTROL         0x65 // Power state control

#define HIGH               1
#define LOW                0

namespace upm {

/**
 * @brief LIDARLITEV3 Optical Distance Measurement Sensor
 * @defgroup lidarlitev3 libupm-lidarlitev3
 * @ingroup garmin i2c light
 */
/**
 * @library lidarlightv3
 * @sensor lidarlightv3
 * @comname Optical Distance Measurement Sensor
 * @type light
 * @man garmin
 * @con i2c
 * @web https://www.sparkfun.com/products/14032
 *
 * @brief API for the LIDARLITEV3 Optical Distance Measurement Sensor
 *
 * It is a compact, high-performance optical distance measurement
 * sensor from Garmin.
 * [LIDARLITEV3 Datasheet]
 * http://static.garmin.com/pumac/LIDAR_Lite_v3_Operation_Manual_and_Technical_Specifications.pdf
 *
 * @snippet lidarlitev3.cxx Interesting
 */
class LIDARLITEV3 : virtual public iDistance {
    public:
        /**
         * Instantiates an LIDARLITEV3 object
         *
         * @param bus Number of the used bus
         * @param devAddr Address of the used I2C device: 0x62
         */
        LIDARLITEV3 (int bus, int devAddr=ADDR);

        /**
         * LIDARLITEV3 object destructor; basically, it closes the I2C connection.
         *
         *~LIDARLITEV3 ();
         * no need for the destructor - the I2c connection will be closed when
         * m_i2ControlCtx variable will be out of context
         **/

        /**
         * Returns distance measurement on success
         * Retruns -1 on failure.
         */
        virtual float getDistance ();

        /**
         * Read
         * Perform I2C read from device.
         *
         * @param reg register address to read from.
         * @param monitorBusyFlag if true, the routine will repeatedly read the status
         *                         register until the busy flag (LSB) is 0.
         */
        uint16_t read(int reg, bool monitorBusyFlag);

        /**
         * Returns the name of the component
         */
        std::string name()
        {
            return m_name;
        }

        /**
         * Reads a one-byte register
         *
         * @param reg Address of the register
         */
        uint8_t i2cReadReg_8 (int reg);

        /**
         * Reads a two-byte register
         *
         * @param reg Address of the register
         */
        uint16_t i2cReadReg_16 (int reg);

        /**
         * Writes to a one-byte register
         *
         * @param reg Address of the register
         * @param value Byte to be written
         */
        mraa::Result i2cWriteReg (uint8_t reg, uint8_t value);

    private:
        std::string m_name;

        int m_controlAddr;
        int m_bus;
        mraa::I2c m_i2ControlCtx;
};

}
