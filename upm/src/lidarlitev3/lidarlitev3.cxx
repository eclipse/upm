/*
 * Author: Saloni Jain <saloni.jain@tcs.com>
 * Author: Niti Rohilla <niti.rohilla@tcs.com>
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
#include <string>
#include <stdexcept>
#include <unistd.h>
#include <stdlib.h>

#include "lidarlitev3.hpp"

using namespace upm;

LIDARLITEV3::LIDARLITEV3 (int bus, int devAddr) : m_i2ControlCtx(bus) {
    m_name = "LIDARLITEV3";

    m_controlAddr = devAddr;
    m_bus = bus;

    mraa::Result ret = m_i2ControlCtx.address(m_controlAddr);
    if (ret != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_address() failed");
    }
}

int
LIDARLITEV3::getDistance () {

    if(i2cWriteReg(ACQ_COMMAND, 0x04) < 0)
        return -1;

    return read(0x8f,true);
}

uint16_t
LIDARLITEV3::read(int reg, bool monitorBusyFlag) {
    int busyFlag = 0; // busyFlag monitors when the device is done with a measurement
    int busyCounter = 0; // busyCounter counts number of times busy flag is checked, for timeout
    uint8_t data;
    uint16_t distance;

    if(monitorBusyFlag) {
        busyFlag = 1; // Begin read immediately if not monitoring busy flag
    }

    while(busyFlag != 0) { // Loop until device is not busy
        // Read status register to check busy flag
        data = i2cReadReg_8 (0x01); // Read register 0x01
        busyFlag = data & 1; // Assign the LSB of the status register to busyFlag

        busyCounter++; // Increment busyCounter for timeout

        // Handle timeout condition, exit while loop and goto bailout
        if(busyCounter > 9999) {
            goto timeout;
        }
    }

    if (busyFlag == 0) {
        // Read bytes to obtain 16-bit measured distance in centimeters
        distance = i2cReadReg_16(0x8f);
    }

    // timeout reports error
    if(busyCounter > 9999) {
        timeout:
        busyCounter = 0;
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": Read timeout");
    }
    return distance;
}

uint16_t
LIDARLITEV3::i2cReadReg_16 (int reg) {
    uint16_t data;

    m_i2ControlCtx.writeByte(reg);

    m_i2ControlCtx.read((uint8_t *)&data, 0x2);

    uint8_t high = (data & 0xFF00) >> 8;
    data = (data << 8) & 0xFF00;
    data |= high;

    return data;
}

uint8_t
LIDARLITEV3::i2cReadReg_8 (int reg) {
    uint8_t data;

    m_i2ControlCtx.writeByte(reg);

    m_i2ControlCtx.read(&data, 0x1);

    return data;
}

mraa::Result
LIDARLITEV3::i2cWriteReg (uint8_t reg, uint8_t value) {
    mraa::Result error = mraa::SUCCESS;

    uint8_t data[2] = { reg, value };
    error = m_i2ControlCtx.write (data, 2);
    if ( error != mraa::SUCCESS)
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_i2c_write() failed");

    return error;
}
