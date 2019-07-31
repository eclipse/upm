/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Credits to Seeed Studeo.
 * Based on Seeed Studeo code example,
 * http://www.seeedstudio.com/wiki/index.php?title=Twig_-_I2C_Color_Sensor_v0.9b.
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

#include "tcs3414cs.hpp"

using namespace upm;

TCS3414CS::TCS3414CS (int bus, int addr) : m_i2Ctx(bus) {
    m_name = "TCS3414CS";

    mraa::Result ret = m_i2Ctx.address(addr);
    if (ret != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) + 
                                    ": m_i2Ctx.address() failed");
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
    mraa::Result error = mraa::SUCCESS;

    error = m_i2Ctx.writeByte (CLR_INT);

    if (error != mraa::SUCCESS) {
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

    m_i2Ctx.writeByte(reg);

    readByte = m_i2Ctx.read(buffer, len);
    return readByte;
}

mraa::Result
TCS3414CS::i2cWriteReg_N (uint8_t reg, unsigned int len, uint8_t * buffer) {
    mraa::Result error = mraa::SUCCESS;

    error = m_i2Ctx.writeByte (reg);
    error = m_i2Ctx.write (buffer, len);

    return error;
}

mraa::Result
TCS3414CS::i2cWriteReg (uint8_t reg, uint8_t data) {
    mraa::Result error = mraa::SUCCESS;

    error = m_i2Ctx.writeByte (reg);
    error = m_i2Ctx.writeByte (data);

    return error;
}
