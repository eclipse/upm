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

#include "max44009.hpp"

using namespace upm;

MAX44009::MAX44009 (int bus, int devAddr) {

    m_maxControlAddr = devAddr;
    i2c = new mraa::I2c(bus);
    i2c->address(m_maxControlAddr);

    // Reset chip to defaults
    status = mraa::SUCCESS;
    reset();
    if (status != mraa::SUCCESS)
        UPM_THROW("config failure");
}

MAX44009::~MAX44009() {
    delete i2c;
}

mraa::Result
MAX44009::reset() {
    uint8_t address[5] = {MAX44009_INT_ENABLE_ADDR, MAX44009_CONFIG_ADDR, \
                                   MAX44009_THR_HIGH_ADDR, MAX44009_THR_LOW_ADDR, \
                                   MAX44009_THR_TIMER_ADDR};
    uint8_t value[5]   = {MAX44009_INT_DISABLED, MAX44009_DEFAULT_CONFIGURATION, \
                                   0xFF, 0x00, 0xFF};
    uint8_t i;
    for (i = 0; i < sizeof(address) && status == mraa::SUCCESS; i++) {
        status = i2c->writeReg(address[i], value[i]);
    }
    return status;
}

uint16_t
MAX44009::getVisibleRaw() {
    uint8_t exponent, mantissa;
    uint8_t data[MAX44009_LUX_LENGTH];
    uint16_t* value = reinterpret_cast<uint16_t*>(&data[0]);

    int length = i2c->readBytesReg(MAX44009_LUX_START_ADDR, data, MAX44009_LUX_LENGTH);

    if(length != MAX44009_LUX_LENGTH)
        UPM_THROW("Read error");

    return *value;
}


double
MAX44009::getVisibleLux() {
    uint16_t rawValue = getVisibleRaw();
    uint8_t rawValueMsb = rawValue >> 8;
    uint8_t rawValueLsb = rawValue & 0xFF;
    uint8_t exponent = (( rawValueMsb & 0xF0 ) >> 4);
    uint8_t mantissa = (( rawValueMsb & 0x0F ) << 4 ) | ( rawValueLsb & 0x0F );

    // Check for overrange condition
    if(exponent == MAX44009_OVERRANGE_CONDITION)
        UPM_THROW("Overrange error");

    return pow((double)2,(double)exponent) * mantissa * 0.045;
}

