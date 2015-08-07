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
#include <stdexcept>

#include "maxds3231m.h"

using namespace upm;

MAXDS3231M::MAXDS3231M (int bus, int devAddr) {
    m_name = "MAXDS3231M";

    m_i2cAddr = devAddr;
    m_bus = bus;

    if (!(m_i2Ctx = mraa_i2c_init(m_bus)))
      {
        throw std::invalid_argument(std::string(__FUNCTION__) + 
                                    ": mraa_i2c_init() failed");
      }

    mraa_result_t ret = mraa_i2c_address(m_i2Ctx, m_i2cAddr);
    if (ret != MRAA_SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) + 
                                    ": mraa_i2c_address() failed");
    }
}

MAXDS3231M::~MAXDS3231M() {
    mraa_i2c_stop(m_i2Ctx);
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

    mraa_i2c_address(m_i2Ctx, m_i2cAddr);
    mraa_i2c_write_byte(m_i2Ctx, reg);

    mraa_i2c_address(m_i2Ctx, m_i2cAddr);
    readByte = mraa_i2c_read(m_i2Ctx, buffer, len);
    return readByte;
}

mraa_result_t
MAXDS3231M::i2cWriteReg_N (uint8_t reg, unsigned int len, uint8_t * buffer) {
    mraa_result_t error = MRAA_SUCCESS;

    error = mraa_i2c_address (m_i2Ctx, m_i2cAddr);
    error = mraa_i2c_write (m_i2Ctx, buffer, len);

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
