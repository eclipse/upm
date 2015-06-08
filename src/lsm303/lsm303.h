/*
 * Author: Brendan Le Foll<brendan.le.foll@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Code based on LSM303DLH sample by Jim Lindblom SparkFun Electronics
 * and the CompensatedCompass.ino by Frankie Chu from SeedStudio
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

#include <string.h>
#include <mraa/i2c.h>
#include <math.h>

namespace upm {

/* LSM303 Address definitions */
#define LSM303_MAG 0x1E // assuming SA0 grounded
#define LSM303_ACC 0x18 // assuming SA0 grounded

/* LSM303 Register definitions */
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
 * @brief LSM303 accelerometer/compass library
 * @defgroup lsm303 libupm-lsm303
 * @ingroup seeed adafruit i2c accelerometer compass
 */

/**
 * @library lsm303
 * @sensor lsm303
 * @comname LSM303 Accelerometer & Compass
 * @altname Grove 6-Axis Accelerometer & Compass
 * @type accelerometer compass
 * @man seeed adafruit
 * @web http://www.seeedstudio.com/wiki/Grove_-_6-Axis_Accelerometer%26Compass
 * @con i2c
 *
 * @brief API for LSM303 Accelerometer & Compass
 *
 * This file defines the LSM303DLH 3-axis magnetometer/3-axis accelerometer.
 * This module was tested with the SeedStudio Grove [6-Axis Accelerometer&Compass]
 * module that is used over i2c. The magnometer and acceleromter are accessed
 * at two seperate i2c addresses.
 *
 * @image html lsm303.jpeg
 * @snippet lsm303.cxx Interesting
 */
class LSM303 {
    public:
         /**
         * Instanciates a LSM303 object
         *
         * @param i2c bus
         * @param addr magometer
     * @param addr accelerometer
         */
        LSM303 (int bus,
                int addrMag=LSM303_MAG,
                int addrAcc=LSM303_ACC,
                int accScale=8);

        /**
         * LSM303 object destructor
         */
        ~LSM303 ();

        /**
         * Get Current Heading, headings <0 indicate an error occured
         *
         * @return float
         */
        float getHeading();

        /**
         * Get the coordinates in XYZ order
         */
        mraa_result_t getCoordinates();

        /**
         * Get accelerometer values
         * Call before calling other "get" functions for acceleration
         */
        mraa_result_t getAcceleration();

        /**
         * Get the raw coordinate data, this will get updated when getCoordinates() is called
         */
        int16_t* getRawCoorData();

        /**
         * Just get the X component of the coordinate data
         */
        int16_t getCoorX();

        /**
         * Just get the Y component of the coordinate data
         */
        int16_t getCoorY();

        /**
         * Just get the Z component of the coordinate data
         */
        int16_t getCoorZ();

        /**
         * Get the raw accelerometer data, this will get updated when getAcceleration() is called
         */
        int16_t* getRawAccelData();

        /**
         * Just get the X component of the acceleration
         */
        int16_t getAccelX();

        /**
         * Just get the Y component of the acceleration
         */
        int16_t getAccelY();

        /**
         * Just get the Z component of the acceleration
         */
        int16_t getAccelZ();

    private:
        int readThenWrite(uint8_t reg);
        mraa_result_t setRegisterSafe(uint8_t slave, uint8_t sregister, uint8_t data);

        mraa_i2c_context m_i2c;
        int m_addrMag;
    int m_addrAcc;
        uint8_t buf[6];
        int16_t coor[3];
        int16_t accel[3];
};

}
