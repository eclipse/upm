/*
 * Author: Scott Ware <scott.r.ware@intel.com>
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
#include <math.h>
#include <errno.h>

#include "si1132.hpp"


#define SI1132_ADDRESS                  0x60

/* REGISTER ADDRESSES */
#define SI1132_REG_PART_ID              0x00
#define SI1132_REG_INT_CFG              0x03
#define SI1132_REG_IRQ_ENABLE           0x04
#define SI1132_REG_HW_KEY               0x07
#define SI1132_REG_MEAS_RATE0           0x08
#define SI1132_REG_MEAS_RATE1           0x09
#define SI1132_REG_PARAM_WR             0x17
#define SI1132_REG_COMMAND              0x18
#define SI1132_REG_RESPONSE             0x20
#define SI1132_REG_ALS_VIS_DATA0        0x22
#define SI1132_REG_ALS_VIS_DATA1        0x23
#define SI1132_REG_PARAM_RD             0x2E
#define SI1132_REG_CHIP_STAT            0x30

/* REGISTER VALUES */
#define SI1132_PART_ID                  0x32
#define SI1132_HW_KEY_INIT              0x17

 /* COMMAND VALUES */
#define SI1132_COMMAND_RESET            0x01
#define SI1132_COMMAND_ALS_FORCE        0x06
#define SI1132_COMMAND_PARAM_QUERY      0x80
#define SI1132_COMMAND_PARAM_SET        0xA0
#define SI1132_COMMAND_ALS_AUTO         0x0E

/* PARAMETER RAM ADDRESSES */
#define SI1132_PARAM_CHLIST             0x01
#define SI1132_PARAM_ALS_VIS_ADC_COUNT  0x10
#define SI1132_PARAM_ALS_VIS_ADC_GAIN   0x11
#define SI1132_PARAM_ALS_VIS_ADC_MISC   0x12

/* PARAMETER RAM VALUES */
#define SI1132_PARAM_CHLIST_ENALSVIS    0x10
#define SI1132_PARAM_ALS_VIS_RANGE_STD  0x00
#define SI1132_PARAM_ALS_VIS_RANGE_HI   0x20



using namespace upm;

SI1132::SI1132 (int bus) {
    i2c = new mraa::I2c(bus);
    i2c->address(SI1132_ADDRESS);

    // Reset chip to defaults
    status = reset();
    if (status != mraa::SUCCESS)
        UPM_THROW("config failure");
}

SI1132::~SI1132() {
   delete i2c;
}

mraa::Result SI1132::reset() {
    i2c->address(SI1132_ADDRESS);

    // Check version
    uint8_t regValue = i2c->readReg(SI1132_REG_PART_ID);
    if (regValue != SI1132_PART_ID)  {
        fprintf(stderr, "SI1132: Read ID failed. Data = %02x\n", regValue);
        status = mraa::ERROR_UNSPECIFIED;
        return status;
    }

    // disable automatic updates
    uint16_t rate = 0;
    status = i2c->writeWordReg(SI1132_REG_MEAS_RATE0, rate);
    if (status != mraa::SUCCESS)  {
        fprintf(stderr, "SI1132_REG_MEAS_RATE0 failed\n");
        return status;
    }

    // reset device
    status = i2c->writeReg(SI1132_REG_COMMAND, SI1132_COMMAND_RESET);
    if (status != mraa::SUCCESS) {
        fprintf(stderr, "SI1132: Reset failed.\n");
        status = mraa::ERROR_UNSPECIFIED;
        return status;
    }
    sleepMs(30);

    // start state machine
    i2c->writeReg(SI1132_REG_HW_KEY, SI1132_HW_KEY_INIT);
    regValue = i2c->readReg(SI1132_REG_HW_KEY);
    if (regValue != SI1132_HW_KEY_INIT) {
        fprintf(stderr, "Si1132: Did not start\n");
        status = mraa::ERROR_UNSPECIFIED;
        return status;
    }

    status = writeParam(SI1132_PARAM_CHLIST, SI1132_PARAM_CHLIST_ENALSVIS);

    // set visible light range for indoor lighting
    status = writeParam(SI1132_PARAM_ALS_VIS_ADC_MISC, SI1132_PARAM_ALS_VIS_RANGE_STD);

    // set visible light gain to 8
    status = writeParam(SI1132_PARAM_ALS_VIS_ADC_GAIN, 3);
    status = writeParam(SI1132_PARAM_ALS_VIS_ADC_COUNT, 3 << 4);
    return status;
}


uint16_t SI1132::getVisibleRaw() {
    status = runCommand(SI1132_COMMAND_ALS_FORCE);
    if (status != mraa::SUCCESS)
       UPM_THROW("command failed");
    return i2c->readWordReg(SI1132_REG_ALS_VIS_DATA0);
}


double SI1132::getVisibleLux() {
    uint16_t rawValue = getVisibleRaw();
    if (rawValue < 256)
        rawValue = 0;
    else
        rawValue -= 256;
    return static_cast<double>(rawValue);
}


mraa::Result SI1132::clearResponseRegister()
{
    uint8_t regValue = 0xFF;
    status = i2c->writeReg(SI1132_REG_COMMAND, 0);
    if (status != mraa::SUCCESS)
        return status;
    int bytesRead = i2c->readBytesReg(SI1132_REG_RESPONSE, &regValue, 1);
    if (bytesRead == 1 && regValue == 0)
       status = mraa::SUCCESS;
    else
        status = mraa::ERROR_UNSPECIFIED;
    return status;
}

mraa::Result SI1132::runCommand(uint8_t command)
{
    uint8_t response = 0;
    int sleepTimeMs = 5;
    int timeoutMs = 50;
    int waitTimeMs = 0;
    i2c->address(SI1132_ADDRESS);
    status = clearResponseRegister();
    if (status != mraa::SUCCESS)
        return status;
    status = i2c->writeReg(SI1132_REG_COMMAND, command);
    if (status != mraa::SUCCESS)
        return status;
    while (response == 0 && waitTimeMs < timeoutMs) {
        response = i2c->readReg(SI1132_REG_RESPONSE);
        sleepMs(sleepTimeMs);
        waitTimeMs += sleepTimeMs;
    }
    if (response == 0) {
        status = mraa::ERROR_UNSPECIFIED;
        fprintf(stderr, "Command %d failed\n", command);
    }
    return status;
}

mraa::Result SI1132::writeParam(uint8_t param, uint8_t value)
{
    i2c->address(SI1132_ADDRESS);
    status = i2c->writeReg(SI1132_REG_PARAM_WR, value);
    if (status != mraa::SUCCESS)
        return status;
    return runCommand(SI1132_COMMAND_PARAM_SET | param);
}

mraa::Result SI1132::readParam(uint8_t param, uint8_t* value)
{
    status = runCommand(SI1132_COMMAND_PARAM_QUERY | param);
    if (status != mraa::SUCCESS)
        return status;
    if (i2c->readBytesReg(SI1132_REG_PARAM_RD, value, 1) != 1)
        status = mraa::ERROR_UNSPECIFIED;
    return status;
}


void SI1132::sleepMs(int mseconds)
{
    struct timespec sleepTime;

    sleepTime.tv_sec = mseconds / 1000; // Number of seconds
    sleepTime.tv_nsec = ( mseconds % 1000 ) * 1000000; // Convert fractional seconds to nanoseconds

    // Iterate nanosleep in a loop until the total sleep time is the original
    // value of the seconds parameter
    while ( ( nanosleep( &sleepTime, &sleepTime ) != 0 ) && ( errno == EINTR ) );
}
