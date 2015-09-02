/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
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
#include <string.h>
#include <pthread.h>
#include <math.h>

#include "mma7455.h"

using namespace upm;

MMA7455::MMA7455 (int bus, int devAddr) : m_i2ControlCtx(bus) {
    unsigned char data   = 0;
    int           nBytes = 0;

    m_name = "MMA7455";

    m_controlAddr = devAddr;
    m_bus = bus;

    mraa::Result error = m_i2ControlCtx.address(m_controlAddr);
    if (error != mraa::SUCCESS) {
        fprintf(stderr, "Messed up i2c bus\n");
        return;
    }

    // setting GLVL 0x1 (64LSB/g) and MODE 0x1 (Measurement Mode)
    data = (BIT (MMA7455_GLVL0) | BIT (MMA7455_MODE0));
    error = ic2WriteReg (MMA7455_MCTL, &data, 0x1);
    if (error != mraa::SUCCESS) {
        std::cout << "ERROR :: MMA7455 instance wan not created (Mode)" << std::endl;
        return;
    }

    if (mraa::SUCCESS != calibrate ()) {
        std::cout << "ERROR :: MMA7455 instance wan not created (Calibrate)" << std::endl;
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

        error = ic2WriteReg (MMA7455_XOFFL,  (unsigned char *) &xyz, 0x6);
        if (error != mraa::SUCCESS) {
            return error;
        }

    } while ( ++i < 3 );

    return error;
}

mraa::Result
MMA7455::readData (short * ptrX, short * ptrY, short * ptrZ) {
    accelData xyz;
    unsigned char data = 0;
    int nBytes = 0;

    /*do {
        nBytes = ic2ReadReg (MMA7455_STATUS, &data, 0x1);
    } while ( !(data & MMA7455_DRDY) && nBytes == mraa::SUCCESS);

    if (nBytes == mraa::SUCCESS) {
        std::cout << "NO_GDB :: 1" << std::endl;
        return mraa::SUCCESS;
    }*/

    nBytes = ic2ReadReg (MMA7455_XOUTL, (unsigned char *) &xyz, 0x6);
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

#ifdef SWIGJAVA
short *MMA7455::readData() {
    short *v = new short[3];
    readData(&v[0], &v[1], &v[2]);
    return v;
}
#endif

int
MMA7455::ic2ReadReg (unsigned char reg, unsigned char * buf, unsigned char size) {
    if (mraa::SUCCESS != m_i2ControlCtx.address(m_controlAddr)) {
        return 0;
    }

    if (mraa::SUCCESS != m_i2ControlCtx.writeByte(reg)) {
        return 0;
    }

    if (mraa::SUCCESS != m_i2ControlCtx.address(m_controlAddr)) {
        return 0;
    }

    return (int) m_i2ControlCtx.read(buf, size);
}

mraa::Result
MMA7455::ic2WriteReg (unsigned char reg, unsigned char * buf, unsigned char size) {
    mraa::Result error = mraa::SUCCESS;

    uint8_t data[size + 1];
    data[0] = reg;
    memcpy(&data[1], buf, size);

    error = m_i2ControlCtx.address (m_controlAddr);
    if (error != mraa::SUCCESS) {
        return error;
    }
    error = m_i2ControlCtx.write (data, size + 1);
    if (error != mraa::SUCCESS) {
        return error;
    }

    return error;
}
