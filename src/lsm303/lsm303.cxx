/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
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

#include <iostream>
#include <unistd.h>
#include <stdlib.h>

#include "lsm303.h"

using namespace upm;

LSM303::LSM303(int bus, int addrMag, int addrAcc)
{
    mraa_result_t ret = MRAA_SUCCESS;

    m_addrMag = addrMag;
    m_addrAcc = addrAcc;

    m_i2c = mraa_i2c_init(bus);

    // 0x27 is the 'normal' mode with X/Y/Z enable
    setRegisterSafe(m_addrAcc, CTRL_REG1_A, 0x27);

    // scale == 2, can be 4 or 8
    setRegisterSafe(m_addrAcc, CTRL_REG4_A, 0x00);

    // 0x10 = minimum datarate ~15Hz output rate
    setRegisterSafe(m_addrMag, CRA_REG_M, 0x10);

    // magnetic scale = +/-1.3
    // Gaussmagnetic scale = +/-1.3Gauss (0x20)
    // +-8.1Gauss (0xe0)
    setRegisterSafe(m_addrMag, CRB_REG_M, 0xe0);

    // 0x00 = continouous conversion mode
    setRegisterSafe(m_addrMag, MR_REG_M, 0x00);
}

LSM303::~LSM303() {
    mraa_i2c_stop(m_i2c);
}

float
LSM303::getHeading()
{
    if (getCoordinates() != MRAA_SUCCESS) {
        return -1;
    }

    float heading = 180 * atan2(coor[Y], coor[X])/M_PI;

    if (heading < 0)
        heading += 360;

    return heading;
}

uint8_t*
LSM303::getRawAccelData()
{
    return &accel[0];
}

uint8_t*
LSM303::getRawCoorData()
{
    return &coor[0];
}

uint8_t
LSM303::getAccelY()
{
  return accel[2];
}

uint8_t
LSM303::getAccelZ()
{
  return accel[0];
}

uint8_t
LSM303::getAccelX()
{
  return accel[1];
}

mraa_result_t
LSM303::getCoordinates()
{
    mraa_result_t ret = MRAA_SUCCESS;

    memset(&buf[0], 0, sizeof(uint8_t)*6);
    ret = mraa_i2c_address(m_i2c, m_addrMag);
    ret = mraa_i2c_write_byte(m_i2c, OUT_X_H_M);
    ret = mraa_i2c_address(m_i2c, m_addrMag);
    int num = mraa_i2c_read(m_i2c, buf, 6);
    if (num != 6) {
        return ret;
    }
    // convert to coordinates
    for (int i=0; i<3; i++) {
        coor[i] = (buf[2*i] << 8) | buf[(2*i)+1];
    }
    // note that coor array is in XZY order
    //printf("X=%x, Y=%x, Z=%x\n", coor[X], coor[Y], coor[Z]);

    return ret;
}

// helper function that writes a value to the acc and then reads
int
LSM303::readThenWrite(uint8_t reg)
{
    mraa_i2c_address(m_i2c, m_addrAcc);
    mraa_i2c_write_byte(m_i2c, reg);
    mraa_i2c_address(m_i2c, m_addrAcc);
    return (int) mraa_i2c_read_byte(m_i2c);
}

mraa_result_t
LSM303::getAcceleration()
{
    mraa_result_t ret = MRAA_SUCCESS;

    accel[2] = (readThenWrite(OUT_X_L_A) << 8) | (readThenWrite(OUT_X_H_A));
    accel[0] = (readThenWrite(OUT_Y_L_A) << 8) | (readThenWrite(OUT_Y_H_A));
    accel[1] = (readThenWrite(OUT_Z_L_A) << 8) | (readThenWrite(OUT_Z_H_A));
    //printf("X=%x, Y=%x, Z=%x\n", accel[X], accel[Y], accel[Z]);

    return ret;
}

// helper function that sets a register and then checks the set was succesful
mraa_result_t
LSM303::setRegisterSafe(uint8_t slave, uint8_t sregister, uint8_t data)
{
    buf[0] = sregister;
    buf[1] = data;
    if (mraa_i2c_address(m_i2c, slave) != MRAA_SUCCESS) {
        fprintf(stderr, "lsm303: Failed to connect to slave\n");
        return MRAA_ERROR_INVALID_HANDLE;
    }
    if (mraa_i2c_write(m_i2c, buf, 2) != MRAA_SUCCESS) {
        fprintf(stderr, "lsm303: Failed to write to register\n");
        return MRAA_ERROR_INVALID_HANDLE;
    }
    uint8_t val = mraa_i2c_read_byte_data(m_i2c, sregister);
    if (val != data) {
        fprintf(stderr, "lsm303: Failed to set register correctly\n");
        return MRAA_ERROR_UNSPECIFIED;
    }
    return MRAA_SUCCESS;
}
