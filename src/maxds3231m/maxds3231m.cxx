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
#include <unistd.h>
#include <stdlib.h>
#include <stdexcept>

#include "maxds3231m.hpp"

using namespace upm;

MAXDS3231M::MAXDS3231M (int bus, int devAddr) : m_i2Ctx(bus) {
    m_name = "MAXDS3231M";

    m_i2cAddr = devAddr;
    m_bus = bus;

    mraa::Result ret = m_i2Ctx.address(m_i2cAddr);
    if (ret != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) + 
                                    ": m_i2Ctx.address() failed");
    }
}

void
MAXDS3231M::setDate (Time3231 &time) {
    uint8_t *data = (uint8_t *)&time;

    i2cWriteReg_N (TIME_CAL_ADDR, 7, data);
}

bool
MAXDS3231M::getDate (Time3231 &time) {
    uint8_t buffer[7];

    // We need 7 bytes of data.
    if (i2cReadReg_N (TIME_CAL_ADDR, 7, buffer) > 6) {
        uint8_t century = (buffer[5] & 0x80) >> 7;

        time.second     = BCDtoDEC(buffer[0]);
        time.minute     = BCDtoDEC(buffer[1]);
        time.hour       = BCDtoDEC(buffer[2]);
        time.day        = BCDtoDEC(buffer[4]);
        time.month      = BCDtoDEC(buffer[5] & 0x1F);
        time.year       = (century == 1) ? 2000 + BCDtoDEC(buffer[6]) : 1900 + BCDtoDEC(buffer[6]);
        time.weekDay    = BCDtoDEC(buffer[3]);

        return true;
    }

    return false;
}

uint16_t
MAXDS3231M::getTemperature () {
    uint8_t     buffer[2];
    uint8_t     msb     = 0;
    uint8_t     lsb     = 0;

    i2cReadReg_N (TEMPERATURE_ADDR, 2, buffer);
    msb = buffer[0];
    lsb = buffer[1] >> 6;

    if ((msb & 0x80) != 0)
        msb |= ~((1 << 8) - 1); // if negative get two's complement

    return 0.25 * lsb + msb;
}

/*
 * **************
 *  private area
 * **************
 */
uint16_t
MAXDS3231M::i2cReadReg_N (int reg, unsigned int len, uint8_t * buffer) {
    int readByte = 0;

    m_i2Ctx.writeByte(reg);

    readByte = m_i2Ctx.read(buffer, len);
    return readByte;
}

mraa::Result
MAXDS3231M::i2cWriteReg_N (uint8_t reg, unsigned int len, uint8_t * buffer) {
    mraa::Result error = mraa::SUCCESS;

    error = m_i2Ctx.write (buffer, len);

    return error;
}

uint8_t
MAXDS3231M::DECtoBSD(uint8_t data) {
    return ((data / 10 * 16) + (data % 10));
}

uint8_t
MAXDS3231M::BCDtoDEC(uint8_t data) {
    return ((data / 16 * 10) + (data % 16));
}
