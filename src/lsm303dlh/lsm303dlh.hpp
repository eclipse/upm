/*
 * Author: Brendan Le Foll<brendan.le.foll@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Code based on LSM303DLH sample by Jim Lindblom SparkFun Electronics
 * and the CompensatedCompass.ino by Frankie Chu from SeedStudio
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <string.h>
#include <mraa/i2c.hpp>
#include <math.h>

namespace upm {

/* LSM303DLH Address definitions */
#define LSM303DLH_MAG 0x1E
#define LSM303DLH_ACC 0x19

/* LSM303DLH Register definitions */
#define CTRL_REG1_A 0x20
#define CTRL_REG2_A 0x21
#define CTRL_REG3_A 0x22
#define CTRL_REG4_A 0x23
#define CTRL_REG5_A 0x24

#define CRA_REG_M 0x00
#define CRB_REG_M 0x01

#define MR_REG_M 0x02
#define OUT_X_H_M 0x03

#define OUT_X_L_A 0x28
#define OUT_X_H_A 0x29
#define OUT_Y_L_A 0x2A
#define OUT_Y_H_A 0x2B
#define OUT_Z_L_A 0x2C
#define OUT_Z_H_A 0x2D

#define X 0
#define Y 1
#define Z 2

/**
 * @brief LSM303DLH Accelerometer/Compass
 * @defgroup lsm303dlh libupm-lsm303dlh
 * @ingroup seeed adafruit stmicro i2c accelerometer compass
 */

/**
 * @library lsm303dlh
 * @sensor lsm303dlh
 * @comname Triaxial Accelerometer/magnetometer
 * @altname Grove 6-Axis Accelerometer & Compass
 * @type accelerometer compass
 * @man seeed adafruit stmicro
 * @web http://www.seeedstudio.com/wiki/Grove_-_6-Axis_Accelerometer%26Compass
 * @con i2c
 *
 * @brief API for the LSM303DLH Accelerometer & Compass
 *
 * This module defines the LSM303DLHDLH 3-axis magnetometer/3-axis accelerometer.
 * This module was tested with the Seeed Studio* Grove 6-Axis Accelerometer & Compass
 * module used over I2C. The magnetometer and acceleromter are accessed
 * at two seperate I2C addresses.
 *
 * @image html lsm303dlh.jpeg
 * @snippet lsm303dlh.cxx Interesting
 */
    class LSM303DLH {
    public:
	typedef enum {
		LSM303DLH_MAG_T = 0,
		LSM303DLH_ACC_T } LSM303DLH_SLAVE_T;
        /**
         * Instantiates an LSM303DLH object
         *
         * @param bus I2C bus to use
         * @param addrMag I2C address of the Magnetometer (default 0x1E)
         * @param addrAcc I2C address of the Accelerometer (default 0x19)
         * @param accScale Accelerometer scale, can be 2, 4 or 8 (default 8)
         */
        LSM303DLH (int bus,
                   int addrMag=LSM303DLH_MAG,
                   int addrAcc=LSM303DLH_ACC,
                   int accScale=8);

        /**
         * LSM303DLH object destructor
         * where is no more need for this here - I2c connection will be stopped
         * automatically when m_i2c variable will go out of scope
         * ~LSM303DLH ();
         **/

        /**
         * Gets the current heading; headings <0 indicate an error has occurred
         *
         * @return float
         */
        float getHeading();

        /**
         * Gets the coordinates in the XYZ order
         */
        mraa::Result getCoordinates();

        /**
         * Gets accelerometer values
         * Should be called before other "get" functions for acceleration
         */
        mraa::Result getAcceleration();

        /**
         * Gets raw coordinate data; it is updated when getCoordinates() is called
         */
        int16_t* getRawCoorData();

        /**
         * Gets the X component of the coordinates data
         */
        int16_t getCoorX();

        /**
         * Gets the Y component of the coordinates data
         */
        int16_t getCoorY();

        /**
         * Gets the Z component of the coordinates data
         */
        int16_t getCoorZ();

        /**
         * Gets raw accelerometer data; it is updated when getAcceleration() is called
         */
        int16_t* getRawAccelData();

        /**
         * Gets the X component of the acceleration data
         */
        int16_t getAccelX();

        /**
         * Gets the Y component of the acceleration data
         */
        int16_t getAccelY();

        /**
         * Gets the Z component of the acceleration data
         */
        int16_t getAccelZ();

    private:
        int readThenWrite(uint8_t reg);
        mraa::Result setRegisterSafe(LSM303DLH_SLAVE_T slave, uint8_t sregister, uint8_t data);

        mraa::I2c m_i2cMag;
        mraa::I2c m_i2cAcc;
        int m_addrMag;
        int m_addrAcc;
        uint8_t buf[6];
        int16_t coor[3];
        int16_t accel[3];
    };

}
