/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
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

#include <iostream>
#include <string>
#include <stdexcept>
#include <unistd.h>
#include <stdlib.h>

#include "lsm303dlh.hpp"

using namespace upm;

LSM303DLH::LSM303DLH(int bus, int addrMag, int addrAcc, int accScale) :
    m_i2cMag(bus), m_i2cAcc(bus)
{
    m_addrMag = addrMag;
    m_addrAcc = addrAcc;


    if (m_i2cMag.address(m_addrMag) != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_address() failed");
    }

    if (m_i2cAcc.address(m_addrAcc) != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_address() failed");
    }

    // 0x27 is the 'normal' mode with X/Y/Z enable
    setRegisterSafe(LSM303DLH_ACC_T, CTRL_REG1_A, 0x27);

    // scale can be 2, 4 or 8
    if (2 == accScale) {
        setRegisterSafe(LSM303DLH_ACC_T, CTRL_REG4_A, 0x00);
    } else if (4 == accScale) {
        setRegisterSafe(LSM303DLH_ACC_T, CTRL_REG4_A, 0x10);
    } else { // default; equivalent to 8g
        setRegisterSafe(LSM303DLH_ACC_T, CTRL_REG4_A, 0x30);
    }

    // 0x10 = minimum datarate ~15Hz output rate
    setRegisterSafe(LSM303DLH_MAG_T, CRA_REG_M, 0x10);

    // magnetic scale = +/-1.3
    // Gaussmagnetic scale = +/-1.3Gauss (0x20)
    // +-8.1Gauss (0xe0)
    setRegisterSafe(LSM303DLH_MAG_T, CRB_REG_M, 0xe0);

    // 0x00 = continouous conversion mode
    setRegisterSafe(LSM303DLH_MAG_T, MR_REG_M, 0x00);
}

float
LSM303DLH::getHeading()
{
    if (getCoordinates() != mraa::SUCCESS) {
        return -1;
    }

    float heading = 180.0 * atan2(double(coor[Y]), double(coor[X]))/M_PI;

    if (heading < 0.0)
        heading += 360.0;

    return heading;
}

int16_t*
LSM303DLH::getRawAccelData()
{
    return &accel[0];
}

int16_t*
LSM303DLH::getRawCoorData()
{
    return &coor[0];
}

int16_t
LSM303DLH::getAccelX()
{
    return accel[X];
}

int16_t
LSM303DLH::getAccelY()
{
    return accel[Y];
}

int16_t
LSM303DLH::getAccelZ()
{
    return accel[Z];
}

mraa::Result
LSM303DLH::getCoordinates()
{
    mraa::Result ret = mraa::SUCCESS;

    memset(&buf[0], 0, sizeof(uint8_t)*6);
    ret = m_i2cMag.writeByte(OUT_X_H_M);
    int num = m_i2cMag.read(buf, 6);
    if (num != 6) {
        return ret;
    }
    // convert to coordinates
    for (int i=0; i<3; i++) {
        coor[i] = (int16_t(buf[2*i] << 8))
            |  int16_t(buf[(2*i)+1]);
    }
    // swap elements 1 and 2 to get things in natural XYZ order
    int16_t t = coor[2];
    coor[2] = coor[1];
    coor[1] = t;
    //printf("X=%x, Y=%x, Z=%x\n", coor[X], coor[Y], coor[Z]);

    return ret;
}

int16_t
LSM303DLH::getCoorX() {
    return coor[X];
}

int16_t
LSM303DLH::getCoorY() {
    return coor[Y];
}

int16_t
LSM303DLH::getCoorZ() {
    return coor[Z];
}

// helper function that writes a value to the acc and then reads
// FIX: shouldn't this be write-then-read?
int
LSM303DLH::readThenWrite(uint8_t reg)
{
    m_i2cAcc.writeByte(reg);
    return (int) m_i2cAcc.readByte();
}

mraa::Result
LSM303DLH::getAcceleration()
{
    mraa::Result ret = mraa::SUCCESS;

    accel[X] = (int16_t(readThenWrite(OUT_X_H_A)) << 8)
        |  int16_t(readThenWrite(OUT_X_L_A));
    accel[Y] = (int16_t(readThenWrite(OUT_Y_H_A)) << 8)
        |  int16_t(readThenWrite(OUT_Y_L_A));
    accel[Z] = (int16_t(readThenWrite(OUT_Z_H_A)) << 8)
        |  int16_t(readThenWrite(OUT_Z_L_A));
    //printf("X=%x, Y=%x, Z=%x\n", accel[X], accel[Y], accel[Z]);

    return ret;
}

// helper function that sets a register and then checks the set was succesful
mraa::Result
LSM303DLH::setRegisterSafe(LSM303DLH_SLAVE_T slave, uint8_t sregister, uint8_t data)
{
    buf[0] = sregister;
    buf[1] = data;
    uint8_t val;

    switch(slave) {
        case LSM303DLH_MAG_T:
            if (m_i2cMag.write(buf, 2) != mraa::SUCCESS) {
                throw std::invalid_argument(std::string(__FUNCTION__) +
                                ": mraa_i2c_write() failed");
                return mraa::ERROR_INVALID_HANDLE;
            }
            val = m_i2cMag.readReg(sregister);
            if (val != data) {
                throw std::invalid_argument(std::string(__FUNCTION__) +
                                ": failed to set register correctly");
                return mraa::ERROR_UNSPECIFIED;
            }
        break;
	case LSM303DLH_ACC_T:
            if (m_i2cAcc.write(buf, 2) != mraa::SUCCESS) {
                throw std::invalid_argument(std::string(__FUNCTION__) +
                                ": mraa_i2c_write() failed");
                return mraa::ERROR_INVALID_HANDLE;
            }
            val = m_i2cAcc.readReg(sregister);
            if (val != data) {
                throw std::invalid_argument(std::string(__FUNCTION__) +
                                ": failed to set register correctly");
                return mraa::ERROR_UNSPECIFIED;
            }
        break;
	default:
	    return mraa::ERROR_UNSPECIFIED;
    }

    return mraa::SUCCESS;
}
