/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Credits to Seeed Studeo.
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

#include "th02.h"

using namespace upm;

struct TH02Exception : public std::exception {
    std::string message;
    TH02Exception (std::string msg) : message (msg) { }
    ~TH02Exception () throw () { }
    const char* what() const throw () { return message.c_str(); }
};

TH02::TH02 () {
    m_name = "TH02";
    m_i2Ctx = mraa_i2c_init(0);

    mraa_result_t ret = mraa_i2c_address(m_i2Ctx, ADDR);
    if (ret != MRAA_SUCCESS) {
        throw TH02Exception ("Couldn't initilize I2C.");
    }
}

TH02::~TH02 () {
    mraa_i2c_stop(m_i2Ctx);
}

float
TH02::getTemperature () {
    uint8_t buffer[3];
    uint16_t temperature = 0;

    /* Start a new temperature conversion */
    i2cWriteReg (REG_CONFIG, CMD_MEASURE_TEMP);

    /* Wait until conversion is done */
    while (getStatus() == false);

    if (i2cReadReg_N (REG_DATA_H, 3, buffer) > 2) {
        temperature = (buffer[1] << 8) | buffer[2];
        temperature >>= 2;

        return (temperature / 32.0) - 50.0;
    }

    return 0.0;
}

float
TH02::getHumidity () {
    uint8_t buffer[3];
    uint16_t humidity = 0;

    /* Start a new humility conversion */
    i2cWriteReg (REG_CONFIG, CMD_MEASURE_HUMI);

    /* Wait until conversion is done */
    while (getStatus() == false);

    if (i2cReadReg_N (REG_DATA_H, 3, buffer) > 2) {
        humidity = (buffer[1] << 8) | buffer[2];
        humidity >>= 4;

        return (humidity / 16.0) - 24.0;
    }

    return 0.0;
}

bool
TH02::getStatus () {
    uint8_t buffer[1];

    if (i2cReadReg_N (REG_STATUS, 1, buffer) > 0) {
        if (buffer[0] & STATUS_RDY_MASK) {
            return true;    // ready
        }
    }

    return false;
}

/*
 * **************
 *  private area
 * **************
 */
uint16_t
TH02::i2cReadReg_N (int reg, unsigned int len, uint8_t * buffer) {
    int readByte = 0;

    if (m_i2Ctx == NULL) {
        throw TH02Exception ("Couldn't find initilized I2C.");
    }

    mraa_i2c_address(m_i2Ctx, ADDR);
    mraa_i2c_write_byte(m_i2Ctx, reg);

    mraa_i2c_address(m_i2Ctx, ADDR);
    readByte = mraa_i2c_read(m_i2Ctx, buffer, len);
    return readByte;
}

mraa_result_t
TH02::i2cWriteReg_N (uint8_t reg, unsigned int len, uint8_t * buffer) {
    mraa_result_t error = MRAA_SUCCESS;

    if (m_i2Ctx == NULL) {
        throw TH02Exception ("Couldn't find initilized I2C.");
    }

    error = mraa_i2c_address (m_i2Ctx, ADDR);
    error = mraa_i2c_write_byte (m_i2Ctx, reg);
    error = mraa_i2c_write (m_i2Ctx, buffer, len);

    return error;
}

mraa_result_t
TH02::i2cWriteReg (uint8_t reg, uint8_t data) {
    mraa_result_t error = MRAA_SUCCESS;

    if (m_i2Ctx == NULL) {
        throw TH02Exception ("Couldn't find initilized I2C.");
    }

    error = mraa_i2c_address (m_i2Ctx, ADDR);
    error = mraa_i2c_write_byte (m_i2Ctx, reg);
    error = mraa_i2c_write_byte (m_i2Ctx, data);

    return error;
}
