/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Credits to Seeed Studeo.
 * Based on Seeed Studeo code example,
 * http://www.seeedstudio.com/wiki/index.php?title=Twig_-_I2C_Color_Sensor_v0.9b.
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

#include "tcs3414cs.h"

using namespace upm;

TCS3414CS::TCS3414CS () {
    m_name = "TCS3414CS";

    if (!(m_i2Ctx = mraa_i2c_init(0)))
      {
        throw std::invalid_argument(std::string(__FUNCTION__) + 
                                    ": mraa_i2c_init() failed");
      }

    mraa_result_t ret = mraa_i2c_address(m_i2Ctx, ADDR);
    if (ret != MRAA_SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) + 
                                    ": mraa_i2c_address() failed");
    }

    // Set timing register
    i2cWriteReg (REG_TIMING, INTEG_MODE_FREE);
    usleep (100000);

    // Set interrupt source register
    i2cWriteReg (REG_INT_SOURCE, INT_SOURCE_GREEN);
    usleep (100000);

    // Set interrupt control register
    i2cWriteReg (REG_INT, INTR_LEVEL | INTR_PERSIST_EVERY);
    usleep (100000);

    // Set gain
    i2cWriteReg (REG_GAIN, GAIN_1 | PRESCALER_4);
    usleep (100000);

    // Enable ADC
    i2cWriteReg (REG_CTL, CTL_DAT_INIITIATE);
    usleep (100000);
}

TCS3414CS::~TCS3414CS () {
    mraa_i2c_stop(m_i2Ctx);
}

void
TCS3414CS::readRGB (tcs3414sc_rgb_t * rgb) {
    uint8_t buffer[8];

    // We need 7 bytes of data.
    if (i2cReadReg_N (REG_BLOCK_READ, 8, buffer) > 7) {
        rgb->g = buffer[1] * 256 + buffer[0];
        rgb->r = buffer[3] * 256 + buffer[2];
        rgb->b = buffer[5] * 256 + buffer[4];
        rgb->clr = buffer[7] * 256 + buffer[6];
    }
}

void
TCS3414CS::clearInterrupt () {
    mraa_result_t error = MRAA_SUCCESS;

    error = mraa_i2c_address (m_i2Ctx, ADDR);
    error = mraa_i2c_write_byte (m_i2Ctx, CLR_INT);

    if (error != MRAA_SUCCESS) {
        throw std::runtime_error(std::string(__FUNCTION__) + 
                                 ": Couldn't clear interrupt");
    }
}

/*
 * **************
 *  private area
 * **************
 */
uint16_t
TCS3414CS::i2cReadReg_N (int reg, unsigned int len, uint8_t * buffer) {
    int readByte = 0;

    mraa_i2c_address(m_i2Ctx, ADDR);
    mraa_i2c_write_byte(m_i2Ctx, reg);

    mraa_i2c_address(m_i2Ctx, ADDR);
    readByte = mraa_i2c_read(m_i2Ctx, buffer, len);
    return readByte;
}

mraa_result_t
TCS3414CS::i2cWriteReg_N (uint8_t reg, unsigned int len, uint8_t * buffer) {
    mraa_result_t error = MRAA_SUCCESS;

    error = mraa_i2c_address (m_i2Ctx, ADDR);
    error = mraa_i2c_write_byte (m_i2Ctx, reg);
    error = mraa_i2c_write (m_i2Ctx, buffer, len);

    return error;
}

mraa_result_t
TCS3414CS::i2cWriteReg (uint8_t reg, uint8_t data) {
    mraa_result_t error = MRAA_SUCCESS;

    error = mraa_i2c_address (m_i2Ctx, ADDR);
    error = mraa_i2c_write_byte (m_i2Ctx, reg);
    error = mraa_i2c_write_byte (m_i2Ctx, data);

    return error;
}
