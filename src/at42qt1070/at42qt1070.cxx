/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
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

#include <unistd.h>
#include <math.h>
#include <iostream>
#include <string>
#include <stdexcept>

#include "at42qt1070.h"

using namespace upm;
using namespace std;


AT42QT1070::AT42QT1070(int bus, uint8_t address)
{
    m_addr = address;

    // setup our i2c link
    if (!(m_i2c = mraa_i2c_init(bus))) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_init() failed");
        return;
    }

    mraa_result_t rv;

    if ((rv = mraa_i2c_address(m_i2c, m_addr)) != MRAA_SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_address() failed");
        return;
    }

    if (readChipID() != 0x2E) {
        throw std::runtime_error("Chip ID does not match the expected value (2Eh)");
    }

    m_buttonStates = 0;
    m_calibrating = false;
    m_overflow = false;
}

AT42QT1070::~AT42QT1070()
{
    mraa_i2c_stop(m_i2c);
}

bool
AT42QT1070::writeByte(uint8_t reg, uint8_t byte)
{
    mraa_result_t rv = mraa_i2c_write_byte_data(m_i2c, byte, reg);

    if (rv != MRAA_SUCCESS) {
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": mraa_i2c_write_byte() failed");
        return false;
    }

    return true;
}

bool
AT42QT1070::writeWord(uint8_t reg, uint16_t word)
{
    mraa_result_t rv = mraa_i2c_write_word_data(m_i2c, word, reg);

    if (rv != MRAA_SUCCESS) {
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": mraa_i2c_write_word() failed");
        return false;
    }

    return true;
}

uint8_t
AT42QT1070::readByte(uint8_t reg)
{
    return mraa_i2c_read_byte_data(m_i2c, reg);
}

uint16_t
AT42QT1070::readWord(uint8_t reg)
{
    return mraa_i2c_read_word_data(m_i2c, reg);
}

uint8_t
AT42QT1070::readChipID(void)
{
    return readByte(REG_CHIPID);
}

void
AT42QT1070::updateState()
{
    uint8_t stat = readByte(REG_DETSTATUS);

    // if we are calibrating, don't change anything
    if (stat & DET_CALIBRATE) {
        m_calibrating = true;
        return;
    } else {
        m_calibrating = false;
    }

    if (stat & DET_OVERFLOW)
        m_overflow = true;
    else
        m_overflow = false;

    // if a touch is occurring, read the button states
    if (stat & DET_TOUCH) {
        uint8_t keys = readByte(REG_KEYSTATUS);
        // high bit is reserved, filter it out
        m_buttonStates = keys & ~0x80;
    } else {
        m_buttonStates = 0;
    }
}

uint8_t
AT42QT1070::getLPMode(void)
{
    return readByte(REG_LP);
}

uint8_t
AT42QT1070::setLPMode(uint8_t mode)
{
    writeByte(REG_LP, mode);

    return getLPMode();
}

uint8_t
AT42QT1070::getAVE(uint8_t key)
{
    uint8_t value, ave;

    if (key > 6) {
        throw std::invalid_argument("Only keys 0-6 are allowed");
    }

    value = readByte(REG_AVE0 + key);
    ave = (value & 0xFC) >> 2;

    return ave;
}

uint8_t
AT42QT1070::setAVE(uint8_t key, uint8_t ave)
{
    uint8_t value;

    if (key > 6) {
        throw std::invalid_argument("Only keys 0-6 are allowed");
    }

    switch (ave) {
        case 1:
        case 2:
        case 4:
        case 8:
        case 16:
        case 32:
            break;

        default:
            throw std::invalid_argument("Invalid averaging factor");
    }

    value = readByte(REG_AVE0 + key);
    value = value & 0x03;
    value = value | (ave << 2);
    writeByte(REG_AVE0 + key, value);

    return getAVE(key);
}

uint8_t
AT42QT1070::getAKSGroup(uint8_t key)
{
    uint8_t value, aks;

    if (key > 6) {
        throw std::invalid_argument("Only keys 0-6 are allowed");
    }

    value = readByte(REG_AVE0 + key);
    aks = value & 0x03;

    return aks;
}

uint8_t
AT42QT1070::setAKSGroup(uint8_t key, uint8_t group)
{
    uint8_t value;

    if (key > 6) {
        throw std::invalid_argument("Only keys 0-6 are allowed");
    }

    if (group > 3) {
        throw std::invalid_argument("Only groups 0-3 are allowed");
    }

    value = readByte(REG_AVE0 + key);
    value = value & 0xFC;
    value = value | group;

    writeByte(REG_AVE0 + key, value);

    return getAKSGroup(key);
}

bool
AT42QT1070::reset()
{
    // write a non-zero value to the reset register
    return writeByte(REG_RESET, 0xff);
}

bool
AT42QT1070::calibrate()
{
    // write a non-zero value to the calibrate register
    return writeByte(REG_CALIBRATE, 0xff);
}
