/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * This driver was rewritten based on the original driver written by:
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 *
 * The MIT License
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <string>
#include <stdexcept>
#include <unistd.h>
#include <stdlib.h>

#include "bmpx8x.hpp"
#include "upm_string_parser.hpp"

using namespace upm;
using namespace std;

BMPX8X::BMPX8X (int bus, int addr) :
    m_bmpx8x(bmpx8x_init(bus, addr))
{
    if (!m_bmpx8x)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmpx8x_init() failed");
}

BMPX8X::BMPX8X(std::string initStr) : mraaIo(initStr)
{
    mraa_io_descriptor* descs = mraaIo.getMraaDescriptors();
    std::vector<std::string> upmTokens;

    if(!mraaIo.getLeftoverStr().empty()) {
        upmTokens = UpmStringParser::parse(mraaIo.getLeftoverStr());
    }

    m_bmpx8x = (bmpx8x_context)malloc(sizeof(struct _bmpx8x_context));
    if(!m_bmpx8x) {
        throw std::runtime_error(std::string(__FUNCTION__)
                                 + ": bmpx8x_init() failed");
    }

    // zero out context
    memset((void *)m_bmpx8x, 0, sizeof(struct _bmpx8x_context));

    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS) {
        bmpx8x_close(m_bmpx8x);
        throw std::runtime_error(std::string(__FUNCTION__)
                                 + ": mraa_init() failed");
    }

    if(!descs->i2cs) {
        throw std::runtime_error(std::string(__FUNCTION__)
                                  + ": mraa_i2c_init() failed");
    } else {
        if( !(m_bmpx8x->i2c = descs->i2cs[0]) ) {
            bmpx8x_close(m_bmpx8x);
            throw std::runtime_error(std::string(__FUNCTION__)
                                 + ": mraa_i2c_init() failed");
        }
    }

    // check the chip id
    uint8_t chipID = bmpx8x_get_chip_id(m_bmpx8x);

    if (chipID != BMPX8X_DEFAULT_CHIPID) {
        printf("%s: invalid chip id: %02x.  Expected %02x\n",
               __FUNCTION__, chipID, BMPX8X_DEFAULT_CHIPID);
        bmpx8x_close(m_bmpx8x);
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmpx8x_init() failed");
    }

    // call devinit with a default ultrahigh resolution mode
    if (bmpx8x_devinit(m_bmpx8x, BMPX8X_OSS_ULTRAHIGHRES)) {
        printf("%s: bmpx8x_devinit() failed.\n", __FUNCTION__);
        bmpx8x_close(m_bmpx8x);
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmpx8x_init() failed");
    }

    std::string::size_type sz;
    for(std::string tok : upmTokens) {
        if(tok.substr(0, 16) == "setOversampling:") {
           BMPX8X_OSS_T oss = (BMPX8X_OSS_T)std::stoi(tok.substr(16), nullptr, 0);
           setOversampling(oss);
        }
        if(tok.substr(0, 9) == "writeReg:") {
            uint8_t reg = std::stoul(tok.substr(9), &sz, 0);
            tok = tok.substr(9);
            uint8_t val = std::stoul(tok.substr(sz+1), nullptr, 0);
            writeReg(reg, val);
        }
    }
}


BMPX8X::~BMPX8X()
{
    bmpx8x_close(m_bmpx8x);
}

void BMPX8X::init(BMPX8X_OSS_T oss)
{
    if (bmpx8x_devinit(m_bmpx8x, oss))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmpx8x_devinit() failed");
}

void BMPX8X::update()
{
    if (bmpx8x_update(m_bmpx8x))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmpx8x_update() failed");
}

void BMPX8X::reset()
{
    if (bmpx8x_reset(m_bmpx8x))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmpx8x_reset() failed");
}

void BMPX8X::setOversampling(BMPX8X_OSS_T oss)
{
    bmpx8x_set_oversampling(m_bmpx8x, oss);
}

uint8_t BMPX8X::readReg(uint8_t reg)
{
    return bmpx8x_read_reg(m_bmpx8x, reg);
}

int BMPX8X::readRegs(uint8_t reg, uint8_t *buffer, int len)
{
    int rv = bmpx8x_read_regs(m_bmpx8x, reg, buffer, len);
    if (rv < 0)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmpx8x_read_regs() failed");

    return rv;
}

void BMPX8X::writeReg(uint8_t reg, uint8_t val)
{
    if (bmpx8x_write_reg(m_bmpx8x, reg, val))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bmpx8x_write_reg() failed");
}

float BMPX8X::getPressure()
{
    return bmpx8x_get_pressure(m_bmpx8x);
}

float BMPX8X::getTemperature()
{
    return bmpx8x_get_temperature(m_bmpx8x);
}

int BMPX8X::getSealevelPressure(float altitudeMeters)
{
    return bmpx8x_get_sealevel_pressure(m_bmpx8x, altitudeMeters);
}

float BMPX8X::getAltitude(int sealevelPressure)
{
    return bmpx8x_get_altitude(m_bmpx8x, sealevelPressure);
}
