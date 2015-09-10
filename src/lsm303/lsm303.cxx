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
#include <string>
#include <stdexcept>
#include <unistd.h>
#include <stdlib.h>

#include "lsm303.h"

using namespace upm;

LSM303::LSM303(int bus, int addrMag, int addrAcc, int accScale) : m_i2c(bus)
{
    m_addrMag = addrMag;
    m_addrAcc = addrAcc;

    // 0x27 is the 'normal' mode with X/Y/Z enable
    setRegisterSafe(m_addrAcc, CTRL_REG1_A, 0x27);

    // scale can be 2, 4 or 8
    if (2 == accScale) {
      setRegisterSafe(m_addrAcc, CTRL_REG4_A, 0x00);
    } else if (4 == accScale) {
      setRegisterSafe(m_addrAcc, CTRL_REG4_A, 0x10);
    } else { // default; equivalent to 8g
      setRegisterSafe(m_addrAcc, CTRL_REG4_A, 0x30);
    }

    // 0x10 = minimum datarate ~15Hz output rate
    setRegisterSafe(m_addrMag, CRA_REG_M, 0x10);

    // magnetic scale = +/-1.3
    // Gaussmagnetic scale = +/-1.3Gauss (0x20)
    // +-8.1Gauss (0xe0)
    setRegisterSafe(m_addrMag, CRB_REG_M, 0xe0);

    // 0x00 = continouous conversion mode
    setRegisterSafe(m_addrMag, MR_REG_M, 0x00);
}

float
LSM303::getHeading()
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
LSM303::getRawAccelData()
{
    return &accel[0];
}

int16_t*
LSM303::getRawCoorData()
{
    return &coor[0];
}

int16_t
LSM303::getAccelX()
{
  return accel[X];
}

int16_t
LSM303::getAccelY()
{
  return accel[Y];
}

int16_t
LSM303::getAccelZ()
{
  return accel[Z];
}

mraa::Result
LSM303::getCoordinates()
{
    mraa::Result ret = mraa::SUCCESS;

    memset(&buf[0], 0, sizeof(uint8_t)*6);
    ret = m_i2c.address(m_addrMag);
    ret = m_i2c.writeByte(OUT_X_H_M);
    ret = m_i2c.address(m_addrMag);
    int num = m_i2c.read(buf, 6);
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
LSM303::getCoorX() {
  return coor[X];
}

int16_t
LSM303::getCoorY() {
  return coor[Y];
}

int16_t
LSM303::getCoorZ() {
  return coor[Z];
}

// helper function that writes a value to the acc and then reads
// FIX: shouldn't this be write-then-read?
int
LSM303::readThenWrite(uint8_t reg)
{
    m_i2c.address(m_addrAcc);
    m_i2c.writeByte(reg);
    m_i2c.address(m_addrAcc);
    return (int) m_i2c.readByte();
}

mraa::Result
LSM303::getAcceleration()
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
LSM303::setRegisterSafe(uint8_t slave, uint8_t sregister, uint8_t data)
{
    buf[0] = sregister;
    buf[1] = data;

    if (m_i2c.address(slave) != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_address() failed");
        return mraa::ERROR_INVALID_HANDLE;
    }
    if (m_i2c.write(buf, 2) != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_write() failed");
        return mraa::ERROR_INVALID_HANDLE;
    }
    uint8_t val = m_i2c.readReg(sregister);
    if (val != data) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": failed to set register correctly");
        return mraa::ERROR_UNSPECIFIED;
    }
    return mraa::SUCCESS;
}
