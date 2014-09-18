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

    buf[0] = CTRL_REG1_A;
    buf[1] = 0x27;
    ret = mraa_i2c_address(m_i2c, m_addrAcc);
    ret = mraa_i2c_write(m_i2c, buf, 2);

    // 0x27 = normal power mode, all accel axes on
    buf[0] = CTRL_REG1_A;
    buf[1] = 0x27;
    ret = mraa_i2c_address(m_i2c, m_addrAcc);
    ret = mraa_i2c_write(m_i2c, buf, 2);

    // scale == 2, can be 4 or 8
    buf[0] = CTRL_REG4_A;
    buf[1] = 0x00;
    ret = mraa_i2c_address(m_i2c, m_addrAcc);
    ret = mraa_i2c_write(m_i2c, buf, 2);

    // 0x14 = mag 30Hz output rate
    buf[0] = CRA_REG_M;
    buf[1] = 0x14;
    ret = mraa_i2c_address(m_i2c, m_addrMag);
    ret = mraa_i2c_write(m_i2c, buf, 2);

    // magnetic scale = +/-1.3Gaussmagnetic scale = +/-1.3Gauss
    buf[0] = CRB_REG_M;
    buf[1] = 0x20; // MAG_SCALE_1_3;
    ret = mraa_i2c_address(m_i2c, m_addrMag);
    ret = mraa_i2c_write(m_i2c, buf, 2);

    // 0x00 = continouous conversion mode
    buf[0] = MR_REG_M;
    buf[1] = 0x00;
    ret = mraa_i2c_address(m_i2c, m_addrMag);
    ret = mraa_i2c_write(m_i2c, buf, 2);
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
        coor[i] = (buf[0*i] << 8) | buf[1*i];
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
