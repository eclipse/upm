/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
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
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "mma7455.hpp"

using namespace upm;

MMA7455::MMA7455 (int bus, int devAddr) : m_i2ControlCtx(bus) {
    unsigned char data   = 0;

    m_name = "MMA7455";

    m_controlAddr = devAddr;
    m_bus = bus;

    mraa::Result error = m_i2ControlCtx.address(m_controlAddr);
    if (error != mraa::SUCCESS) {
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": mraa_i2c_address() failed");
        return;
    }

    // setting GLVL 0x1 (64LSB/g) and MODE 0x1 (Measurement Mode)
    data = (BIT (MMA7455_GLVL0) | BIT (MMA7455_MODE0));
    error = i2cWriteReg (MMA7455_MCTL, &data, 0x1);
    if (error != mraa::SUCCESS) {
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": writing mode register failed");
        return;
    }

    if (mraa::SUCCESS != calibrate ()) {
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": calibrate() failed");
        return;
    }
}

mraa::Result
MMA7455::calibrate () {
    mraa::Result error = mraa::SUCCESS;
    int i = 0;

    accelData xyz;
    xyz.value.x = xyz.value.y = xyz.value.z = 0;

    do {
        error = readData (&xyz.value.x, &xyz.value.y, &xyz.value.z);
        if (mraa::SUCCESS != error) {
            return error;
        }

        xyz.value.x += 2 * -xyz.value.x;
        xyz.value.y += 2 * -xyz.value.y;
        xyz.value.z += 2 * -(xyz.value.z - 64);

        error = i2cWriteReg (MMA7455_XOFFL,  (unsigned char *) &xyz, 0x6);
        if (error != mraa::SUCCESS) {
            return error;
        }

    } while ( ++i < 3 );

    return error;
}

mraa::Result
MMA7455::readData (short * ptrX, short * ptrY, short * ptrZ) {
    accelData xyz;
    int nBytes = 0;

    /*do {
        nBytes = i2cReadReg (MMA7455_STATUS, &data, 0x1);
    } while ( !(data & MMA7455_DRDY) && nBytes == mraa::SUCCESS);

    if (nBytes == mraa::SUCCESS) {
        std::cout << "NO_GDB :: 1" << std::endl;
        return mraa::SUCCESS;
    }*/

    nBytes = i2cReadReg (MMA7455_XOUTL, (unsigned char *) &xyz, 0x6);
    if (nBytes == 0) {
        std::cout << "NO_GDB :: 2" << std::endl;
        return mraa::ERROR_UNSPECIFIED;
    }

    if (xyz.reg.x_msb & 0x02) {
        xyz.reg.x_msb |= 0xFC;
    }

    if (xyz.reg.y_msb & 0x02) {
        xyz.reg.y_msb |= 0xFC;
    }

    if (xyz.reg.z_msb & 0x02) {
        xyz.reg.z_msb |= 0xFC;
    }

    // The result is the g-force in units of 64 per 'g'.
    *ptrX = xyz.value.x;
    *ptrY = xyz.value.y;
    *ptrZ = xyz.value.z;

    return mraa::SUCCESS;
}

std::vector<short> MMA7455::readData() {
    std::vector<short> v(3);
    readData(&v[0], &v[1], &v[2]);
    return v;
}

std::vector<float> MMA7455::getAcceleration() {
    std::vector<float> v(3);
     accelData xyz;
    int nBytes = 0;

    /*do {
        nBytes = i2cReadReg (MMA7455_STATUS, &data, 0x1);
    } while ( !(data & MMA7455_DRDY) && nBytes == mraa::SUCCESS);

    if (nBytes == mraa::SUCCESS) {
        std::cout << "NO_GDB :: 1" << std::endl;
        return mraa::SUCCESS;
    }*/

    nBytes = i2cReadReg (MMA7455_XOUTL, (unsigned char *) &xyz, 0x6);
    if (nBytes == 0) {
        std::cout << "NO_GDB :: 2" << std::endl;
        //return mraa::ERROR_UNSPECIFIED;
    }

    if (xyz.reg.x_msb & 0x02) {
        xyz.reg.x_msb |= 0xFC;
    }

    if (xyz.reg.y_msb & 0x02) {
        xyz.reg.y_msb |= 0xFC;
    }

    if (xyz.reg.z_msb & 0x02) {
        xyz.reg.z_msb |= 0xFC;
    }

    // The result is the g-force in units of 64 per 'g'.
    v[0] = (float)xyz.value.x;
    v[1] = (float)xyz.value.y;
    v[2] = (float)xyz.value.z;

    return v;
}

int
MMA7455::i2cReadReg (unsigned char reg, uint8_t *buffer, int len) {
    if (mraa::SUCCESS != m_i2ControlCtx.writeByte(reg)) {
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": mraa_i2c_write_byte() failed");
        return 0;
    }

    return (int) m_i2ControlCtx.read(buffer, len);
}

mraa::Result
MMA7455::i2cWriteReg (unsigned char reg, uint8_t *buffer, int len) {
    mraa::Result error = mraa::SUCCESS;

    uint8_t data[len + 1];
    data[0] = reg;
    memcpy(&data[1], buffer, len);

    error = m_i2ControlCtx.write (data, len + 1);
    if (error != mraa::SUCCESS) {
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": mraa_i2c_write() failed");
        return error;
    }

    return error;
}
