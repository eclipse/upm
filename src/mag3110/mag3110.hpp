/*
 * Author: Norbert Wesp <nwesp@phytec.de>
 * Copyright (c) 2017 Phytec Messtechnik GmbH.
 *
 * based on: RIOT-driver mag3110 by Johann Fischer <j.fischer@phytec.de>
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <interfaces/iMagnetometer.hpp>

#include <string>
#include <mraa/i2c.hpp>
#include <stdint.h>
#include <stdbool.h>

#define MAG3110_NAME "MAG3110"
#define MAG3110_I2C_ADDRESS   0x0E
#define MAG3110_DEVICE_ID     0xC4
#define MAG3110_DEVICE_ID_REG 0x07

/* MAG3110 Register Map */
#define MAG3110_DR_STATUS            0x00 /**< Data ready status per axis */
#define MAG3110_OUT_X_MSB            0x01 /**< Bits [15:8] of X measurement */
#define MAG3110_OUT_X_LSB            0x02 /**< Bits [7:0] of X measurement */
#define MAG3110_OUT_Y_MSB            0x03 /**< Bits [15:8] of Y measurement */
#define MAG3110_OUT_Y_LSB            0x04 /**< Bits [7:0] of Y measurement */
#define MAG3110_OUT_Z_MSB            0x05 /**< Bits [15:8] of Z measurement */
#define MAG3110_OUT_Z_LSB            0x06 /**< Bits [7:0] of Z measurement */
#define MAG3110_SYSMOD               0x08 /**< FIFO Status Register */
#define MAG3110_OFF_X_MSB            0x09 /**< Bits [15:8] of user X offset */
#define MAG3110_OFF_X_LSB            0x0A /**< Bits [7:0] of user X offset */
#define MAG3110_OFF_Y_MSB            0x0B /**< Bits [15:8] of user Y offset */
#define MAG3110_OFF_Y_LSB            0x0C /**< Bits [7:0] of user Y offset */
#define MAG3110_OFF_Z_MSB            0x0D /**< Bits [15:8] of user Z offset */
#define MAG3110_OFF_Z_LSB            0x0E /**< Bits [7:0] of user Z offset */
#define MAG3110_DIE_TEMP             0x0F /**< Temperature, signed 8 bits */
#define MAG3110_CTRL_REG1            0x10 /**< Operation modes */
#define MAG3110_CTRL_REG2            0x11 /**< Operation modes */

/* MAG3110 DR-STATUS Register */
#define MAG3110_DR_STATUS_ZYXOW           (1 << 7)
#define MAG3110_DR_STATUS_ZOW             (1 << 6)
#define MAG3110_DR_STATUS_YOW             (1 << 5)
#define MAG3110_DR_STATUS_XOW             (1 << 4)
#define MAG3110_DR_STATUS_ZYXDR           (1 << 3)
#define MAG3110_DR_STATUS_ZDR             (1 << 2)
#define MAG3110_DR_STATUS_YDR             (1 << 1)
#define MAG3110_DR_STATUS_XDR             (1 << 0)

/* MAG3110 SYSMOD Register */
#define MAG3110_SYSMOD_STANDBY            0
#define MAG3110_SYSMOD_ACTIVE_RAW         1
#define MAG3110_SYSMOD_ACTIVE             2

/* MAG3110 Control Register 1 */
#define MAG3110_CTRL_REG1_DROS_SHIFT      3
#define MAG3110_CTRL_REG1_DROS_MASK       0xF8
#define MAG3110_CTRL_REG1_DROS(x)         (((uint8_t)(((uint8_t)(x))<<\
                                          MAG3110_CTRL_REG1_DROS_SHIFT))\
                                          &MAG3110_CTRL_REG1_DROS_MASK)
#define MAG3110_CTRL_REG1_FR              (1 << 2)
#define MAG3110_CTRL_REG1_TM              (1 << 1)
#define MAG3110_CTRL_REG1_AC              (1 << 0)

/* MAG3110 Control Register 2 */
#define MAG3110_CTRL_REG2_AUTO_MRST_EN    (1 << 7)
#define MAG3110_CTRL_REG2_RAW             (1 << 5)
#define MAG3110_CTRL_REG2_MAG_RST         (1 << 4)

/* MAG3110 Output Rate (DR) and Over Sample (OS) Ratio for CTRL_REG1 */
#define MAG3110_DROS_8000_16     0 /* DR 80   Hz, OS Ratio 16 */
#define MAG3110_DROS_4000_32     1 /* DR 40   Hz, OS Ratio 32 */
#define MAG3110_DROS_2000_64     2 /* DR 20   Hz, OS Ratio 64 */
#define MAG3110_DROS_1000_128    3 /* DR 10   Hz, OS Ratio 128 */
#define MAG3110_DROS_4000_16     4 /* DR 40   Hz, OS Ratio 16 */
#define MAG3110_DROS_2000_32     5 /* DR 20   Hz, OS Ratio 32 */
#define MAG3110_DROS_1000_64     6 /* DR 10   Hz, OS Ratio 64 */
#define MAG3110_DROS_0500_128    7 /* DR 5    Hz, OS Ratio 128 */
#define MAG3110_DROS_2000_16     8 /* DR 20   Hz, OS Ratio 16 */
#define MAG3110_DROS_1000_32     9 /* DR 10   Hz, OS Ratio 32 */
#define MAG3110_DROS_0500_64    10 /* DR 5    Hz, OS Ratio 64 */
#define MAG3110_DROS_0250_128   11 /* DR 2.5  Hz, OS Ratio 128 */
#define MAG3110_DROS_1000_16    12 /* DR 10   Hz, OS Ratio 16 */
#define MAG3110_DROS_0500_32    13 /* DR 5    Hz, OS Ratio 32 */
#define MAG3110_DROS_0250_64    14 /* DR 2.5  Hz, OS Ratio 64 */
#define MAG3110_DROS_0125_128   15 /* DR 1.25 Hz, OS Ratio 128 */
#define MAG3110_DROS_0500_16    16 /* DR 5    Hz, OS Ratio 16 */
#define MAG3110_DROS_0250_32    17 /* DR 2.5  Hz, OS Ratio 32 */
#define MAG3110_DROS_0125_64    18 /* DR 1.25 Hz, OS Ratio 64 */
#define MAG3110_DROS_0063_128   19 /* DR 0.63 Hz, OS Ratio 128 */
#define MAG3110_DROS_0250_16    20 /* DR 2.5  Hz, OS Ratio 16 */
#define MAG3110_DROS_0125_32    21 /* DR 1.25 Hz, OS Ratio 32 */
#define MAG3110_DROS_0063_64    22 /* DR 0.63 Hz, OS Ratio 64 */
#define MAG3110_DROS_0031_128   23 /* DR 0.31 Hz, OS Ratio 128 */
#define MAG3110_DROS_0125_16    24 /* DR 1.25 Hz, OS Ratio 16 */
#define MAG3110_DROS_0063_32    25 /* DR 0.63 Hz, OS Ratio 32 */
#define MAG3110_DROS_0031_64    26 /* DR 0.31 Hz, OS Ratio 64 */
#define MAG3110_DROS_0016_128   27 /* DR 0.16 Hz, OS Ratio 128 */
#define MAG3110_DROS_0063_16    28 /* DR 0.63 Hz, OS Ratio 16 */
#define MAG3110_DROS_0031_32    29 /* DR 0.31 Hz, OS Ratio 32 */
#define MAG3110_DROS_0016_64    30 /* DR 0.16 Hz, OS Ratio 64 */
#define MAG3110_DROS_0008_128   31 /* DR 0.08 Hz, OS Ratio 128 */
#define MAG3110_DROS_DEFAULT    MAG3110_DROS_0125_128 
                                /* Default Setting for testing*/

namespace upm {

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
    uint8_t status;
    int8_t dtemp;
} mag3110_data_t;

/**
 * @brief MAG3110 Three-Axis Digital Magnetometer
 * @defgroup mag3110 libupm-mag3110
 * @ingroup freescale i2c accelerometer compass
 */

/**
 * @library mag3110
 * @sensor mag3110
 * @comname MAG3110 Three-Axis Digital Magnetometer
 * @type accelerometer compass
 * @man freescale
 * @web http://www.nxp.com/assets/documents/data/en/data-sheets/MAG3110.pdf
 * @con i2c
 *
 * @brief API for the MAG3110 Three-Axis Digital Magnetometer
 *
 * Description in web-link above:
 * The MAG3110 is a small, low-power digital 3D magnetic sensor with a wide
 * dynamic range to allow operation in PCBs with high extraneous magnetic
 * fields.
 * It measures the components of the local magnetic field, the sum of the
 * geomagnetic field and the magnetic field created by components on the
 * circuit board.
 * It can be used in conjunction with a 3-axis accelerometer so that
 * orientation-independent accurate compass heading information may be achieved
 * It is capable of measuring local magnetic fields up to 10 Gauss with output
 * data rates up to 80 Hz.
 *
 * @snippet mag3110.cxx Interesting
 */
class MAG3110: virtual public iMagnetometer {
    public:
        /**
         *
         * Instantiates an MAG3110 object
         *
         * @param bus Number of the used bus
         * @param dros Data rate and over sampling selection
         * @param devAddr Address of the used I2C device
         */
        MAG3110 (int bus, uint8_t dros=MAG3110_DROS_DEFAULT,
                             int devAddr=MAG3110_I2C_ADDRESS);

        /**
         * Check device_id of sensor
         *
         * @return  0 on success
         *         -1 on error
         */
        int checkID(void);

        /**
         * Return magnetometer data in micro-Teslas (uT) in the form
         * of a floating point vector.  update() must have been called
         * prior to calling this method.
         *
         * @return A floating point vector containing x, y, and z in
         * that order.
         */
        std::vector<float> getMagnetometer();

        /**
         * Set user offset correction
         * Offset correction register will be erased after accelerometer reset
         *
         * @param x Offset correction value for x-axis
         * @param y Offset correction value for y-axis
         * @param z Offset correction value for z-axis
         *
         * @return  0 on success
         *         -1 on error
         */
        int setUserOffset(int16_t x, int16_t y, int16_t z);

        /**
         * Set active mode, this enables periodic measurements
         *
         * @return  0 on success
         *         -1 on error
         */
        int setActive(void);

        /**
         * Set standby mode, this disables periodic measurements
         *
         * @return  0 on success
         *         -1 on error
         */
        int setStandby(void);

        /**
         * Check for new set of measurement data.
         *
         * @return >0 if x-, y- and z-axis new sample is ready
         *          0 if measurement is in progress
         */
        int isReady(void);

        /**
         * Read magnetometer's data and saves them to variables
         *
         * Info: To get the actual values for the magnetic field someone
         * has to divide the returned values from the magnetometer by 10
         *
         * @return  0 on success
         *         -1 on error
         */
        int sampleData(void);

        /**
         * Get x-axis magnetic field strength
         *
         * @param bSampleData Flag to read sensor
         * @return The x-axis magnetic field strength on success
         *         -999 on error
         */
        int16_t getX(int bSampleData = 0);

        /**
         * Get y-axis magnetic field strength
         *
         * @param bSampleData Flag to read sensor
         * @return The y-axis magnetic field strength on success
         *         -999 on error
         */
        int16_t getY(int bSampleData = 0);

        /**
         * Get z-axis magnetic field strength
         *
         * @param bSampleData Flag to read sensor
         * @return The z-axis magnetic field strength on success
         *         -999 on error
         */
        int16_t getZ(int bSampleData = 0);

        /**
         * Get value of status register
         *
         * @return Value of status register
         */
        uint8_t getStatus(void);

        /**
         * Get die temperature
         *
         * @return Die temperature
         */
        int8_t getDieTemperature(void);

        /**
         * Get sensor values
         *
         * @param data Sensor values
         * @param bSampleData Flag to read sensor
         * @return  0 on success
         *         -1 on error
         */
        int getData(mag3110_data_t* data, int bSampleData = 0);

    private:

        std::string m_name;

        int m_controlAddr;
        int m_bus;
        mraa::I2c m_i2ControlCtx;

        mag3110_data_t s_data[1];
};

}
